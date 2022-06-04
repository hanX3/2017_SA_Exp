#!/usr/bin/bash

if [ $# -ne 2 ];then
  echo "need 2 parameter."
  echo "such as: ./batch.sh 50 0"
  echo "analysis R50_0"
  exit 1
fi

cd cut
./cut ${1} ${2}

cd ..
