#!/bin/bash

if [ "$1" = "stp" ];then
  rm -fr src matulpat
  cp -R stop src
  make run

elif [ "$1" = "shld" ];then
  rm -fr src matulpat
  cp -R shield src
  make run

elif [ "$1" = "rcvr" ];then
  rm -fr src matulpat
  cp -R recovery src
  make run

elif [ "$1" = "tt" ];then
  rm -fr src matulpat
  cp -R to_table src
  make run

else
  echo "Wrong argument"
  echo -e "Try:\n- 'shld' for shield\n- 'stp' for stop\n- 'rcvr' for recovery\n- 'tt' for to table"
fi
