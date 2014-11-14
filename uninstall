#!/bin/bash

set -e

readonly LIB_DIR="$1"

function usage {
  echo "Usage: $0 <arduino lib directory>"
  echo "   eg: $0 ~/Documents/Arduino/libraries"
}

if [ -z "${LIB_DIR}" ]; then
  usage
  exit 1
fi

for library in `ls -1 .`; do
  if [ -d "$library" ]; then
    echo unlinking $LIB_DIR/$library...
    rm -f $LIB_DIR/$library
  fi
done
