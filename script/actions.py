'''
Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
You cannot use, modify or redistribute this code without explicit
permission from the copyright holder.
-----
File: /script/actions.py
Last Modified: Sunday, 29th November 2020 10:26:43 pm
Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
'''

import argparse
import multiprocessing
import subprocess
import sys
import os
from enum import IntEnum, unique


@unique
class Phase(IntEnum):
    CONFIGURE = 0
    COMPILE = 1
    RUN = 2

def main():
    phases = list(Phase.__members__.keys())
    phase_flags = ['--' + name.lower().replace('_', '-') for name in phases]
    phase_enabled = [False for _ in phases]
    phase_args = [[] for _ in phases]
    recording_phase = -1
    src_dir = ""
    obj_dir = ""

    i = 0
    while True:
        i += 1
        if i == len(sys.argv):
            break
        arg = sys.argv[i]

        if arg in phase_flags:
            for j in range(len(phase_flags)):
                if phase_flags[j] == arg:
                    phase_enabled[j] = True
                    recording_phase = j
                    break
        elif arg in ("-s", "--src-dir"):
            i += 1
            src_dir = sys.argv[i]
        elif arg in ("-o", "--obj-dir"):
            i += 1
            obj_dir = sys.argv[i]
        elif recording_phase >= 0:
            phase_args[recording_phase].append(arg)
        else:
            raise ValueError("Command line argument not recognised '" + arg + "'")

    global abs_src_dir, abs_obj_dir
    # Get absolute path to source directory
    abs_src_dir = os.path.abspath(src_dir if src_dir else os.path.join(__file__, "../.."))
    # Get absolute path to build directory
    abs_obj_dir = os.path.abspath(obj_dir) if obj_dir else os.path.join(abs_src_dir, "build")

    for i in range(len(phases)):
        if not phase_enabled[i]:
            continue
        cmd = [sys.executable, os.path.join(abs_src_dir, "script/{}.py".format(phases[i].lower()))]
        cmd.extend(["-s", abs_src_dir, "-o", abs_obj_dir])
        cmd.extend(['"{}"'.format(a) for a in phase_args[i]])
        subprocess.check_call(" ".join(cmd), shell=True)
    return True


if __name__ == "__main__":
    ret = main()
    exit_code = 0 if ret else 1
    sys.exit(exit_code)
