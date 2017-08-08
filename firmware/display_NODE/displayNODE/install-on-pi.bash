#!/usr/bin/env bash

host=$1
if [[ $# -eq 0 ]] ; then
    echo 'arg1 should be IP or hostname to raspberryboard'
    host=raspberrypi
    echo "assuming:'$host'"
fi

if [[ $# -eq 2 ]] ;  then
    PW=$2
else
    PW="raspberry"
    echo "no password given as arg2, assuming: '$PW'"
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cmd="sshpass -p $PW rsync -avz "$DIR/"dashboard  pi@"$host":/home/pi/dashboard"
echo $cmd
echo "$( $cmd )"
cmd="sshpass -p $PW rsync -avz "$DIR/"webroot  pi@"$host":/home/pi/dashboard/"
echo $cmd
echo "$( $cmd )"

