#!/bin/bash

set -e

readonly ARD_DIR="$1"

function usage {
  echo "Usage: $0 <arduino sketch directory>"
  echo "   eg: $0 ~/Documents/Arduino"
}

if [ -z "${ARD_DIR}" ]; then
  usage
  exit 1
fi

for sketch in `ls -1 .`; do
  if [ -d "$sketch" ]; then
    echo linking $ARD_DIR/$sketch...
    ln -nfs $PWD/$sketch $ARD_DIR/$sketch
  fi
done
