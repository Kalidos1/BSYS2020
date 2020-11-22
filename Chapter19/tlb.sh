#!/bin/bash

numPages="$1"
trials="$2"


for counter in {1..50}
do
    "$(dirname)"./tlb "$numPages" "$trials"
    numPages=$((numPages*2))
    trials=$((numPages*2))
done