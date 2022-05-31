#!/usr/bin/bash

index=9

cd decode
./decode 57 ${index}
./assemble 57 ${index}

cd ../sort
./alignment 57 ${index}
./sort 57 ${index}

cd ../build
./build 57 ${index}

cd ../cut
./cut 57 ${index}

cd ..
