#/bin/env bash

set -xe

qemu-system-x86_64 -drive format=raw,file=build/lenos.bin
