import argparse
import multiprocessing
import os
import subprocess
import json

from sqlalchemy.orm import sessionmaker
from sqlalchemy.engine import create_engine
from algo_proj.DB_objects.base import Base
from algo_proj.DB_objects.password_setting import PasswordSetting, HashType
from algo_proj.DB_objects.work_range import WorkRange, Status
from algo_proj.DB_objects.dict_words import DictWord
from algo_proj.DB_objects.dictionaries import Dictionary
from algo_proj.utils.DB_utils import session_factory_scope, session_instance_scope


def get_work(session):
    with session_instance_scope(session):
        res = session.query(WorkRange, PasswordSetting).join(PasswordSetting). \
            filter(WorkRange.status == Status.free, ~PasswordSetting.is_done). \
            with_for_update().first()
        if not res:
            return None, None
        work_range, pass_settings = res
        work_range.status = Status.in_progress
    return work_range, pass_settings


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="algo_proj worker subprocess manager - opens multiple subprocesses that independently"
                    " query the DB for work and do it.")
    parser.add_argument('exe_path', type=str, help='path to the C worker exe')
    parser.add_argument('--split', type=int, help='largest size per cpp worker', default=0)
    parser.add_argument('--max_subproc', type=int, help='maximum number of subprocesses opened',
                        default=multiprocessing.cpu_count())
    parser.add_argument('--host', type=str, help='DB host address', default='localhost')
    parser.add_argument('--port', type=int, help='DB port', default=3306)
    parser.add_argument('--user', type=str, help='DB user', default='root')
    parser.add_argument('--pass', type=str, help='DB password', default='pass', dest='pass_')
    parser.add_argument('--DB_name', type=str, help='database name', default='algo_proj')
    return parser.parse_args()


def open_iter_subprocess(exe_path, work_range, pass_settings, i, split):
    print(pass_settings.pass_params)
    return subprocess.Popen([str(obj) for obj in
                             [exe_path,
                              i,
                              min(work_range.end, i + split),
                              pass_settings.hash_type.value,
                              json.dumps(json.loads(pass_settings.pass_params)),
                              pass_settings.hash]],
                            stdout=subprocess.PIPE)


def fetch_data_and_run(args):
    # TODO: a way to handle failing - in case execution ends mark ranges as free
    engine = create_engine(
        'mysql+pymysql://{}:{}@{}:{}/{}'.format(args.user, args.pass_, args.host, args.port, args.DB_name))
    session_factory = sessionmaker(bind=engine)
    Base.metadata.create_all(engine)
    session = session_factory()

    while True:
        work_range, pass_settings = get_work(session)
        if not work_range:
            return
        if args.split == 0:
            args.split = work_range.end - work_range.start

        for i in range(work_range.start, work_range.end, args.split):
            p = open_iter_subprocess(args.exe_path, work_range, pass_settings, i, args.split)
            p.wait()
            res = p.communicate()[0]
            if len(res) == 0:
                print('process failed!')
                continue
            res = json.loads(res)
            print(res)
            with session_instance_scope(session):
                work_range.status = Status.done
                if res['result']:
                    pass_settings.is_done = True
                    pass_settings.complete_pass = res['password']
                    return


def manage_subprocesses(args):
    processes = []
    for i in range(args.max_subproc):
        p = multiprocessing.Process(target=fetch_data_and_run, args=(args,))
        processes.append(p)
        p.start()
    for p in processes:
        p.join()


def load_dicts_to_files(args):
    engine = create_engine(
        'mysql+pymysql://{}:{}@{}:{}/{}'.format(args.user, args.pass_, args.host, args.port, args.DB_name))
    session_factory = sessionmaker(bind=engine)
    Base.metadata.create_all(engine)
    with session_factory_scope(session_factory) as session:
        dicts = session.query(Dictionary).all()
        for dict in dicts:
            if os.path.isfile(os.path.join(os.path.dirname(args.exe_path), dict.name)):
                continue
            with open(os.path.join(os.path.dirname(args.exe_path), dict.name), 'w') as f:
                print(dict.name)
                f.writelines(row.word + '\n' for row in session.query(DictWord).filter(DictWord.dict == dict).all())


if __name__ == '__main__':
    args = parse_arguments()
    load_dicts_to_files(args)
    manage_subprocesses(args)
    print("out of work!")
