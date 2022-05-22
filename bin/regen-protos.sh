#!/usr/bin/env bash

set -e

echo "This script requires https://jpa.kapsi.fi/nanopb/download/ version 0.4.5 to be located in the"
echo " root directory if the following step fails, you should download the correct"
echo "prebuilt binaries for your computer into nanopb-0.4.5"

# the nanopb tool seems to require that the .options file be in the current directory!
cd protobufs
../nanopb-0.4.5/generator-bin/protoc --nanopb_out=-v:../src/generated -I=../protobufs *.proto
