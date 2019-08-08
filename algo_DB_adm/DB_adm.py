import argparse
import json
import os
import time

from sqlalchemy.orm import sessionmaker
from sqlalchemy.engine import create_engine
from algo_proj.DB_objects.base import Base, SQLALCHEMY_FORMAT_STRING
from algo_proj.DB_objects.password_setting import PasswordSetting
from algo_proj.DB_objects.work_range import WorkRange, Status
from algo_proj.utils.DB_utils import session_factory_scope
from algo_proj.DB_objects.dictionaries import Dictionary
from algo_proj.DB_objects.dict_words import DictWord
from algo_proj.DB_objects.found_passwords import FoundPasswords


def clean_finished_work(session):
    session.query(WorkRange).filter(WorkRange.status == Status.done).delete(synchronize_session=False)


def get_passwords(pass_id, session):
    return session.query(FoundPasswords.password). \
        filter(FoundPasswords.pass_settings_id == pass_id).all()


def get_unfinished_work(pass_id, session):
    return session.query(WorkRange). \
        filter(WorkRange.status == Status.free,
               WorkRange.pass_settings_id == pass_id). \
        first()


def gen_ranges(start, end, split, password_settings):
    for i in range(start, end, split):
        yield WorkRange(i, min(end, i + split), password_settings)


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="DB manager for brute-forcing passwords - fills a DB with rows workers can access")
    parser.add_argument('start', type=int)
    parser.add_argument('end', type=int)
    parser.add_argument('hash_structure', type=str, help='a json describing the hash structure')
    parser.add_argument('password_structure', type=str, help='a json describing the password structure')
    parser.add_argument('split', type=int, help='largest size per worker')
    parser.add_argument('--host', type=str, help='DB host address', default='localhost')
    parser.add_argument('--port', type=int, help='DB port', default=3306)
    parser.add_argument('--user', type=str, help='DB user', default='root')
    parser.add_argument('--pass', type=str, help='DB password', default='pass', dest='pass_')
    parser.add_argument('--DB_name', type=str, help='database name', default='algo_proj')
    parser.add_argument('--dicts_path', type=str, help='location for dictionaries', default=None)
    return parser.parse_args()


def load_dicts(session_factory, dicts_path):
    for filename in os.listdir(dicts_path):
        if os.path.isdir(filename):
            continue
        with session_factory_scope(session_factory) as session:
            next_dict = session.query(Dictionary).filter(Dictionary.name == filename).first()
            if next_dict is not None:
                continue
            with open(os.path.join(dicts_path, filename), 'r') as f:
                dictionary = Dictionary(filename)
                session.add(dictionary)
                session.add_all((DictWord(word.strip(), dictionary) for word in f))


def main():
    args = parse_arguments()
    engine = create_engine(
        SQLALCHEMY_FORMAT_STRING.format(args.user, args.pass_, args.host, args.port, args.DB_name))
    session_factory = sessionmaker(bind=engine)
    Base.metadata.create_all(engine)

    if args.dicts_path is not None:
        load_dicts(session_factory, args.dicts_path)

    with session_factory_scope(session_factory) as session:
        clean_finished_work(session)

    with session_factory_scope(session_factory) as session:
        # TODO: write hash_type less somehow.
        pass_json = json.dumps(json.loads(args.password_structure.replace("'", '"')))
        hash_json = json.dumps(json.loads(args.hash_structure.replace("'", '"')))
        password_settings = PasswordSetting(pass_json.replace('\'', '"'), hash_json.replace('\'', '"'))
        session.add(password_settings)
        session.flush()  # so we can get ID
        pass_id = password_settings.id
        session.add_all(gen_ranges(args.start, args.end, args.split, password_settings))

    return wait_for_completion(session_factory, pass_id)


def wait_for_completion(session_factory, pass_id):
    print('done setting up')
    curr_res = []
    while True:  # TODO: another way?
        time.sleep(5)
        with session_factory_scope(session_factory) as session:
            res = get_passwords(pass_id, session)
            if len(res) > len(curr_res):
                print("found passwords: {}".format([r[0] for r in res][len(curr_res):]))
                curr_res = res
            future_work = get_unfinished_work(pass_id, session)
            if not future_work:
                print("checked all passwords, exiting.")
                break
    return curr_res


if __name__ == '__main__':
    main()
