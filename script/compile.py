'''
Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
You cannot use, modify or redistribute this code without explicit
permission from the copyright holder.
-----
File: /script/compile.py
Last Modified: Sunday, 29th November 2020 10:26:49 pm
Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
'''

import argparse
import multiprocessing
import subprocess
import sys
import os

DEFAULT_CPU_COUNT = 4

def do_compile(args):
    # Get job count
    if args.jobs:
        cpu_count = args.jobs
    else:
        try:
            cpu_count = multiprocessing.cpu_count()
        except NotImplementedError:
            cpu_count = DEFAULT_CPU_COUNT

    # Get absolute path to source directory
    abs_src_dir = os.path.abspath(args.src_dir if args.src_dir else os.path.join(__file__, "../.."))
    # Get absolute path to build directory
    abs_obj_dir = os.path.abspath(args.obj_dir) if args.obj_dir else os.path.join(abs_src_dir, "build")

    cmake_cmd = [
        "cmake",
        "--build", abs_obj_dir,
        "-j", str(cpu_count)]

    print("[Cmake Command]: {}".format(" ".join(cmake_cmd)))

    subprocess.check_call(cmake_cmd, cwd=abs_obj_dir)

    return True


def main():
    parser = argparse.ArgumentParser(prog="compile.py",
                                     description="script to do compile",
                                     formatter_class=argparse.RawTextHelpFormatter)

    parser.add_argument("-s", "--src-dir", metavar="SRC_DIR", help="source directory")
    parser.add_argument("-o", "--obj-dir", metavar="OBJ_DIR", help="build directory")
    parser.add_argument("-j", "--jobs", metavar="JOB_CNT", help="Specify number of jobs (default is core count, falling back to 4).")

    args = parser.parse_args()

    print("args:{}".format(args))

    return do_compile(args)


if __name__ == "__main__":
    ret = main()
    exit_code = 0 if ret else 1
    sys.exit(exit_code)
