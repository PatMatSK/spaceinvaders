#!/bin/bash

make clean

if [ "$1" = "stp" ];then
  cp -R stop src
  make run

elif [ "$1" = "shld" ];then
  cp -R shield src
  make run

elif [ "$1" = "rcvr" ];then
  cp -R recovery src
  make run

elif [ "$1" = "tt" ];then
  cp -R to_table src
  make run

elif [ "$1" = "do" ];then
  cp -R deleteObstacle src
  make run

else
  echo "Wrong argument"
  echo -e "Try:\n- 'shld' for shield\n- 'stp' for stop\n- 'rcvr' for recovery\n- 'tt' for to table\n- 'do' for delete obstacle"
fi
