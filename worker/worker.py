import argparse
import os
import subprocess
import tempfile

# TODO: something better as an enum
HashType = {'id': 0}
PassType = {'chars': 0, 'nums': 1}


def parse_arguments():
    parser = argparse.ArgumentParser(description="worker for brute-forcing passwords")
    parser.add_argument('start', type=int)
    parser.add_argument('end', type=int)
    parser.add_argument('hash_type', choices=HashType.keys())
    parser.add_argument('pass_type', choices=PassType.keys())
    parser.add_argument('hash', type=str, help='hashed password')
    parser.add_argument('exe_path', type=str, help='path to the C worker exe')
    parser.add_argument('--split', type=int, help='largest size per worker', default=0)
    parser.add_argument('--max_subproc', type=int, help='maximum number of subprocesses opened', default=4)
    return parser.parse_args()


def open_process(args, i, split):
    return subprocess.Popen([str(obj) for obj in
                             [args.exe_path,
                              i,
                              min(args.end, i + split),
                              HashType[args.hash_type],
                              PassType[args.pass_type],
                              args.hash]],
                            stdout=subprocess.PIPE)


def subprocess_search(args):
    if args.split == 0:
        split = args.end - args.start
    else:
        split = args.split
    print(args.hash)

    processes = []
    for i in range(args.start, args.end, split):
        processes.append(open_process(args, i, split))
        if len(processes) >= args.max_subproc:
            for p in processes:
                p.wait()
                res = p.communicate()[0]
                if res[0] == b'1'[0]:
                    print('found password: {}'.format(res[2:]))
                    return res[2:]
            processes = []
    print('failed!')
    return None


if __name__ == '__main__':
    args = parse_arguments()
    subprocess_search(args)
