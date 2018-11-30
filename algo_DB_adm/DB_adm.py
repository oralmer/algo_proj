import argparse
import inspect

from sqlalchemy.orm import sessionmaker
from sqlalchemy.engine import create_engine
from algo_proj.DB_objects.base import Base
from algo_proj.DB_objects.password_setting import PasswordSetting, HashType
from algo_proj.DB_objects.work_range import WorkRange
from algo_proj.utils.DB_utils import session_scope


def enum_to_dict(enum):
    return {pair[0]: pair[1].value for pair in
            inspect.getmembers(enum, lambda a: not (inspect.isroutine(a))) if
            all([pair[0][0:2] != '__', pair[0][-2:] != '__', pair[0] != 'name', pair[0] != 'value'])}


hash_type = enum_to_dict(HashType)
# proper PassType support delayed due to imminent deprecation and replacement
pass_type = {'chars': 0, 'nums': 1}


def gen_ranges(start, end, split, password_settings):
    for i in range(start, end, split):
        yield WorkRange(i, min(args.end, i + split), password_settings)


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="DB manager for brute-forcing passwords - fills a DB with rows workers can access")
    parser.add_argument('start', type=int)
    parser.add_argument('end', type=int)
    parser.add_argument('hash_type', choices=hash_type.keys())
    parser.add_argument('pass_type', choices=pass_type.keys())
    parser.add_argument('split', type=int, help='largest size per worker')
    parser.add_argument('hash', type=str, help='hashed password')
    parser.add_argument('--host', type=str, help='DB host address', default='localhost')
    parser.add_argument('--port', type=int, help='DB port', default=3306)
    parser.add_argument('--user', type=str, help='DB user', default='root')
    parser.add_argument('--pass', type=str, help='DB password', default='pass', dest='pass_')
    parser.add_argument('--DB_name', type=str, help='database name', default='algo_proj')
    return parser.parse_args()


if __name__ == '__main__':
    args = parse_arguments()
    print(args)
    engine = create_engine(
        'mysql+pymysql://{}:{}@{}:{}/{}'.format(args.user, args.pass_, args.host, args.port, args.DB_name))
    Session = sessionmaker(bind=engine)
    Base.metadata.create_all(engine)
    with session_scope(Session) as session:
        # TODO: write hash_type less somehow.
        # pass_type is a string in preparation for next iteration
        password_settings = PasswordSetting(str(args.pass_type), HashType(hash_type[args.hash_type]), args.hash)
        session.add(password_settings)
        session.add_all(gen_ranges(args.start, args.end, args.split, password_settings))
