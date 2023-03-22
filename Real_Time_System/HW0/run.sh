#!/usr/local/bin/bash

array=()

for i in $(seq 1 10000); do
	array+=(`./main`)
done

echo ${array[@]} >> res
