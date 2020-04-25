#!/bin/sh
DIR=$(dirname $1)
APP=$(basename $1)

mkdir $DIR/lib/
ldd $1 | grep "=> /" | awk '{print $3}' | xargs -I '{}' cp -v '{}' $DIR/lib/

cp $(dirname $0)/linux-run.sh "$1.sh"
chmod +x "$1.sh"
