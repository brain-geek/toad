#!/bin/bash
# Arguments = -u http://brain-geek.github.com/ -c 10 -v

usage()
{
cat << EOF
usage: $0 options

  8..8
 (----)
( >__< )
 ^^~~^^

This script runs the Toad website load testing project.

OPTIONS:
   -h      Show this message
   -u      URL to start crawling
   -c      Number of browsers per process (default - 5)
   -p      Number of processes (default - 1)
   -o      Directory for logs (default - logs disabled)
EOF
}

URL=
CONCURRENCY=5
PROCESSES=1
LOG_DIR=

while getopts “hc:p:u:o:” OPTION
do
     case $OPTION in
         h)
             usage
             exit 1
             ;;
         u)
             URL=$OPTARG
             ;;
         c)
             CONCURRENCY=$OPTARG
             ;;
         p)
             PROCESSES=$OPTARG
             ;;
         o)
             LOG_DIR=$OPTARG
             ;;
         ?)
             usage
             exit
             ;;
     esac
done

if [[ -z $URL ]]
then
     usage
     exit 1
fi

if [[ $LOG_DIR ]]; then
  mkdir -p $(dirname $LOG_DIR)
fi

trap "killall toad" EXIT

#starting PROCESSES - 1 toads in background!
let PROCESSES--

for i in $(seq $PROCESSES); do
  if [[ $LOG_DIR ]]; then
     ./toad $URL $CONCURRENCY $LOG_DIR $LOG_DIR-$i &
     echo "toad cmd: ./toad $URL $CONCURRENCY $LOG_DIR $LOG_DIR-$i "
  else
     ./toad $URL $CONCURRENCY &
  fi
done

if [[ $LOG_DIR ]]; then
   ./toad $URL $CONCURRENCY $LOG_DIR-0
else
   ./toad $URL $CONCURRENCY
fi
