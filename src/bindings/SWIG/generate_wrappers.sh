#!/usr/bin/env bash

if [ $# -lt 2 ]; then
  echo "Missing required arguments"
  echo "Usage: ./generate_wrappers.sh REPOSITORY TARGET_LANGUAGE"

  exit
fi

WORKING_DIR=$1
CURRENT_DIR=`pwd`
LANG=$2

cd $WORKING_DIR;
`swig -Wall -${LANG} -c++ -module ${LANG}_karazeh -I../../../include -Ipackages -o "wrappers/${LANG}_karazeh_wrap.cxx" "packages/karazeh.i"`
cd $CURRENT_DIR;