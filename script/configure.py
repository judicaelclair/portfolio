'''
Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
You cannot use, modify or redistribute this code without explicit
permission from the copyright holder.
-----
File: /script/configure.py
Last Modified: Sunday, 29th November 2020 10:26:36 pm
Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
'''

import argparse
import os
import platform
import subprocess
import sys
import shutil

def do_configure(args):
    # Get absolute path to source directory
    abs_src_dir = os.path.abspath(args.src_dir if args.src_dir else os.path.join(__file__, "../.."))
    # Get absolute path to build directory
    abs_obj_dir = os.path.abspath(args.obj_dir) if args.obj_dir else os.path.join(abs_src_dir, "build")
    # Create build directory if it doesn't exist
    if not os.path.isdir(abs_obj_dir):
        os.makedirs(abs_obj_dir)

    werror = 'ON'
    if args.no_werror:
        werror = 'OFF'

    cmake_cmd = ["cmake"]

    if args.cmake_gen:
        cmake_cmd.extend(["-G", args.cmake_gen])

    cmake_cmd.extend([
        "-S", abs_src_dir,
        "-B", abs_obj_dir,
        "-DWARNINGS_AS_ERRORS={}".format(werror),
        "-DCMAKE_BUILD_TYPE={}".format(args.build_type),
    ])

    # Add additional CMake options if provided
    if args.cmake_flags:
        cmake_cmd.extend(("-D" + s for s in args.cmake_flags))

    # Add path to root CMakeLists.txt
    cmake_cmd.append(abs_src_dir)

    print("[Cmake Command]: {}".format(" ".join(cmake_cmd)))

    cmake_cache = os.path.join(abs_obj_dir, "CMakeCache.txt")
    if os.path.isfile(cmake_cache):
        os.remove(cmake_cache)

    subprocess.check_call(cmake_cmd, cwd=abs_obj_dir)
    return True

def main():
    parser = argparse.ArgumentParser(prog="configure.py",
                                     description="Generate build files from CMake configuration files",
                                     formatter_class=argparse.RawTextHelpFormatter)
    # User options
    parser.add_argument("-s", "--src-dir", metavar="SRC_DIR", help="source directory (autodetected by default)")
    parser.add_argument("-o", "--obj-dir", metavar="OBJ_DIR", help="build directory. (<src>/build by default)")
    parser.add_argument("-t", "--build-type",
                        metavar="BUILD_TYPE", default="RelWithDebInfo", help="build type: Debug, Release, MinSizeRel, RelWithDebInfo")
    parser.add_argument("-c", "--clear", action='store_true', help="Wipes build directory clean")
    parser.add_argument("--no-werror", action='store_true', help="Don't treat warnings as errors")
    parser.add_argument("-D", dest="cmake_flags", metavar="CMAKE_FLAG", action='append', help="Additional CMake option not configured via script parameters")
    parser.add_argument("-G", dest="cmake_gen", default="Ninja" if shutil.which("ninja") != None else "", help="CMake generator")

    args = parser.parse_args()

    print("args:{}".format(args))

    return do_configure(args)


if __name__ == "__main__":
    ret = main()
    exit_code = 0 if ret else 1
    sys.exit(exit_code)
