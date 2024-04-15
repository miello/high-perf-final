#!/bin/bash

make

folder_out="out_$(date +%s)"

if [ ! -d PowerGrid ]; then
    unzip testcases.zip
fi

if [ ! -d "$folder_out" ]; then
    mkdir $folder_out
else 
    rm -rf $folder_out/*
fi

success=()
failed=()

for filename in PowerGrid/*; do
    echo "=============================================================="
    name=$(basename "$filename")
    echo "$name"
    timeout 5s time -p ./power_grid "$filename" "$folder_out/$name.out"
    if [ ! -s "$folder_out/$name.out" ]; then
        echo "Failed: Timeout"
        failed+=("$name")
        continue
    fi

    ./validator "$filename" "$folder_out/$name.out"

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

python3 ./gen_report.py "$folder_out" "report_$folder_out"