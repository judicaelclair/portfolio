#!/bin/bash
set -e

# Brief: Build project with optional actions to do before or after build.
# !! WARNING !! -- must be run from root folder - i.e. CompilerNerd/<here>

## ARGUMENTS
# 
# -e / --emscripten [[path_to_emcc]]
#   Specify path to emscripten compiler. Note, will overide current cmake build cache (if any).
#
# -c / --clean
#   Builds clean.
#
# -C / --clear
#   Removes build director and reconfigures from scratch.
#
# -j / --jobs <#N>
#   Specify number of jobs (default is 4).
#
# -d / --deploy
#   Deploys build
#
# -r / --run-local-server
#   Runs a local server. Type 'localhost:8000' in any browser once local http server is running to view webapp.
#
# -D.*
#   Pass cmake argument.
#
# --no-build
#   Skip build step.

num_args=$#
num_jobs=4
do_clean=false
do_clear=false
deploy=false
run_server_locally=false
cmake_args=""
build=true
for ((i=1;i<=$num_args;i++)); do
  arg=${!i}
  if [ $arg == "-e" ] || [ $arg == "--emscripten" ]; then
    arg=$((i+1))
    EMCC="${!arg}"
  elif [ $arg == "-c" ] || [ $arg == "--clean" ]; then
    do_clean=true
  elif [ $arg == "-C" ] || [ $arg == "--clear" ]; then
    do_clear=true
  elif [ $arg == "-j" ] || [ $arg == "--jobs" ]; then
    arg=$((i+1))
    num_jobs="${!arg}"
  elif [ $arg == "-d" ] || [ $arg == "--deploy" ]; then
    deploy=true
  elif [ $arg == "-r" ] || [ $arg == "--run-local-server" ]; then
    run_server_locally=true
  elif [[ $arg =~ -D.* ]]; then
    cmake_args="$cmake_args $arg"
    # --> here can do some processing on cmake arg <-- #
  elif [[ $arg == "--no-build" ]]; then
    build=false
  fi
done

# BUILD (+ clean & clear)
if [[ "$do_clear" == "true" ]]; then
  rm -rf build
elif [[ "$do_clean" == "true" ]]; then
  if [[ -d "build" ]]; then
    cd build
    make clean || true
    cd ..
  fi
fi

if [[ "$build" == "true" ]]; then
  mkdir -p build
  cd build
  
  if [[ "${cmake_args}" != "" ]]; then
    rm -f CMakeCache.txt
  fi

  cmake -j$num_jobs $cmake_args ..
  make

  # COPY OUTPUT FILES TO WELL-DEFINED LOCATIONS: 'build/out/<here>'.
  rm -rf out
  mkdir -p out/webapp
  cd out/webapp
  mv ../../webapp* .   # move artefacts
  cp ../../../html/* . # copy html data
fi

# DEPLOY
if [[ "$deploy" == "true" ]]; then
  sshpass -f '/.imperial_pwd' ssh -o stricthostkeychecking=no jsc18@shell1.doc.ic.ac.uk "rm -rf /homes/jsc18/public_html/{,.[!.],..?}*"
  sshpass -f '/.imperial_pwd' scp -o stricthostkeychecking=no -r $PWD/* jsc18@shell1.doc.ic.ac.uk:/homes/jsc18/public_html
fi

# RUN LOCALLY
if [[ "$run_server_locally" == "true" ]]; then
  python3 -m http.server
fi
