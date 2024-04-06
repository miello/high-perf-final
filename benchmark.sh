#!/bin/sh

if [ ! -f power_grid ] || [ ! -f validator ]; then
    make
fi

if [ ! -d PowerGrid ]; then
    unzip testcases.zip
fi

if [ ! -d out ]; then
    mkdir out
fi

for filename in PowerGrid/*; do
    name=${filename##*/}
	echo $name
	time -p ./power_grid $filename out/$name.out
    echo "\n"
    ./validator $filename out/$name.out
    echo "\n"
done