#!/bin/bash

numPages="$1"
trials="$2"


for counter in {1..10}
do
    echo "NEUE ITERATION"
    echo "$numPages"
    echo "$trials"
    ./tlb "$numPages" "$trials"
    numPages=$((numPages*2))
done
