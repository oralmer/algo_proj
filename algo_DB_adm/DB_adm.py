import argparse
import inspect
import os
import time

from sqlalchemy.orm import sessionmaker
from sqlalchemy.engine import create_engine
from algo_proj.DB_objects.base import Base, SQLALCHEMY_FORMAT_STRING
from algo_proj.DB_objects.password_setting import PasswordSetting, HashType
from algo_proj.DB_objects.work_range import WorkRange, Status
from algo_proj.utils.DB_utils import session_factory_scope
from algo_proj.DB_objects.dictionaries import Dictionary
from algo_proj.DB_objects.dict_words import DictWord



def clean_finished_work(session):
    session.query(WorkRange).filter(WorkRange.pass_settings_id == PasswordSetting.id,
                                    PasswordSetting.is_done).delete(synchronize_session=False)


def get_password(pass_id, session):
    return session.query(PasswordSetting). \
        filter(PasswordSetting.id == pass_id, PasswordSetting.is_done).first()


def get_unfinished_work(pass_id, session):
    return session.query(WorkRange). \
        filter(WorkRange.status == Status.free,
               WorkRange.pass_settings_id == pass_id). \
        first()


def enum_to_dict(enum):
    """turns an enum class into a name:value dict"""
    return {member_name_val[0]: member_name_val[1].value for member_name_val in
            inspect.getmembers(enum, lambda a: not (inspect.isroutine(a))) if
            all([member_name_val[0][0:2] != '__',
                 member_name_val[0][-2:] != '__',
                 member_name_val[0] != 'name',
                 member_name_val[0] != 'value'])}


hash_type = enum_to_dict(HashType)


def gen_ranges(start, end, split, password_settings):
    for i in range(start, end, split):
        yield WorkRange(i, min(end, i + split), password_settings)


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="DB manager for brute-forcing passwords - fills a DB with rows workers can access")
    parser.add_argument('start', type=int)
    parser.add_argument('end', type=int)
    parser.add_argument('hash_type', choices=hash_type.keys())
    parser.add_argument('password_structure', type=str, help='a json describing the password structure')
    parser.add_argument('split', type=int, help='largest size per worker')
    parser.add_argument('hash', type=str, help='hashed password')
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

    if (args.dicts_path is not None):
        load_dicts(session_factory, args.dicts_path)

    with session_factory_scope(session_factory) as session:
        clean_finished_work(session)

    with session_factory_scope(session_factory) as session:
        # TODO: write hash_type less somehow.
        pass_json = args.password_structure
        password_settings = PasswordSetting(pass_json.replace('\'', '"'), HashType(hash_type[args.hash_type]),
                                            args.hash)
        session.add(password_settings)
        session.flush()  # so we can get ID
        pass_id = password_settings.id
        session.add_all(gen_ranges(args.start, args.end, args.split, password_settings))

    return wait_for_completion(session_factory, pass_id)


def wait_for_completion(session_factory, pass_id):
    print('done setting up')
    while True:  # TODO: another way?
        time.sleep(5)
        with session_factory_scope(session_factory) as session:
            res = get_password(pass_id, session)
            if res:
                print("done! password is: {}".format(res.complete_pass))
                break
            future_work = get_unfinished_work(pass_id, session)
            if not future_work:
                print("couldn't find password!")
                break
    return res.complete_pass


if __name__ == '__main__':
    main()
