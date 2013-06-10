#!/bin/bash

IP=$1
FOLDER=$2

mkdir -p $FOLDER

for i in $(seq 30); do
  echo "Running $(($i)) browsers per process experiment (using 4 processes)"

  wget http://$IP/sitemap
  xvfb-run ./toad -u http://$IP -t 180 -p 4 -c $(($i)) -o $FOLDER/log$i.log
done
