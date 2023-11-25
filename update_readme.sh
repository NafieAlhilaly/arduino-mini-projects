#! /bin/bash

for i in $@
do
    read_from=$(cat $i/README.md  | grep -nE "^--$" | head -1 | cut -d : -f 1)
    read_to=$(cat $i/README.md | grep -nE "^--$" | tail -1 | cut -d : -f 1)
    echo "$(cat $i/README.md | sed -n "$read_from,$read_to p" |  sed 's\--\\')"
done > README.md