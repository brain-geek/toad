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
EOF
}

URL=
CONCURRENCY=5
PROCESSES=1

while getopts “hc:p:u:” OPTION
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

trap "killall toad" EXIT

#starting PROCESSES - 1 toads in background!
let PROCESSES--

for i in $(seq $PROCESSES); do
   ./toad $URL $CONCURRENCY &
done

./toad $URL $CONCURRENCY
