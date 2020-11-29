'''
Copyright (C) 2020 Judicaël CLAIR - All Rights Reserved
You cannot use, modify or redistribute this code without explicit
permission from the copyright holder.
-----
File: /script/run.py
Last Modified: Sunday, 29th November 2020 10:26:30 pm
Modified By: Judicaël CLAIR <clair.judicael@gmail.com>
'''

import argparse
import subprocess
import os
import sys

g_abs_obj_dir = None
g_abs_src_dir = None
g_abs_html_dir = None

def run_command(cmd):
    print(cmd)
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, shell=True)
    output = p.communicate()[0].decode("utf-8")
    if p.returncode != 0:
        print("FAILED: %d\n%s" % (p.returncode, output), file=sys.stderr)
        sys.exit(p.returncode)

def run(args):
    if args.deploy:
        run_command("sshpass -f /.imperial_pwd ssh -o stricthostkeychecking=no jsc18@" + args.deploy + ".doc.ic.ac.uk \"rm -rf /homes/jsc18/public_html/{,.[!.],..?}*\"")
        run_command("sshpass -f /.imperial_pwd scp -o stricthostkeychecking=no -r {}/* jsc18@{}.doc.ic.ac.uk:/homes/jsc18/public_html".format(g_abs_html_dir, args.deploy))

    if args.local:
        run_command("cd {} && {} -m http.server".format(g_abs_html_dir, sys.executable))


def main():
    parser = argparse.ArgumentParser(prog="multiple_runs.py",
                                     formatter_class=argparse.RawTextHelpFormatter)

    # Standard user options
    parser.add_argument("-s", "--src-dir", metavar="SRC_DIR", help="source directory (autodetected by default)")
    parser.add_argument("-o", "--obj-dir", metavar="OBJ_DIR", help="build directory. (<src>/build by default)")

    # Special user options
    parser.add_argument("-l", "--local", action="store_true", help="run local server")
    parser.add_argument("-d", "--deploy", nargs='?', default=False, const="shell1", help="deploy to Imperial College server through <deploy>.doc.ic.ac.uk")

    args = parser.parse_args()

    global g_abs_src_dir, g_abs_obj_dir, g_abs_html_dir
    # Get absolute path to source directory
    g_abs_src_dir = os.path.abspath(args.src_dir if args.src_dir else os.path.join(__file__, "../.."))
    # Get absolute path to build directory
    g_abs_obj_dir = os.path.abspath(args.obj_dir) if args.obj_dir else os.path.join(g_abs_src_dir, "build")
    # Get absolute html directory
    g_abs_html_dir = os.path.join(g_abs_obj_dir, "public_html")

    run(args)


if __name__ == "__main__":
    main()
