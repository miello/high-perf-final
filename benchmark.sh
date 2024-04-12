#!/bin/bash

if [ ! -f power_grid ] || [ ! -f validator ]; then
    make
fi

if [ ! -d PowerGrid ]; then
    unzip testcases.zip
fi

if [ ! -d out ]; then
    mkdir out
fi

success=()
failed=()

for filename in PowerGrid/*; do
    echo "=============================================================="
    name=$(basename "$filename")
    echo "$name"
    timeout 60s time -p ./power_grid "$filename" "out/$name.out"
    if [ ! -s "out/$name.out" ]; then
        echo "Failed: Timeout"
        failed+=("$name")
        continue
    fi

    ./validator "$filename" "out/$name.out"

    echo "\n"

    success+=("$name")
done

echo "=============================================================="

echo "Success: ${#success[@]}"
for item in "${success[@]}"; do
    echo "$item"
done

echo "=============================================================="

echo "Failed: ${#failed[@]}"
for item in "${failed[@]}"; do
    echo "$item"
done