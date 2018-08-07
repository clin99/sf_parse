#!/bin/bash
for folder in /home/clin99/Software/OpenTimer/benchmark/*; do 
  name=${folder##*/}
  file="$folder/$name.spef"

  if [ -f "$file" ]; then 
    this_file=${file##*/}
    length=${#this_file}
    let "pad = 30 - length"

    printf "Parse $this_file %30s"
    /usr/bin/time -f "\t%E real,\t%U user,\t%S sys" ./a.out $file
    if [ $? -ne 0 ]; 
    then 
      echo "Fail on $file"
      exit 1
    fi 
#    exit 0
  fi
done
