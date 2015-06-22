#!/bin/bash

file=$1

cols=`cat $file | awk "{} END { print NF }"`

{
echo -n "plot \"$file\" u 1 w l"
for (( i=2; i<=$cols; i++))
do
  ltype=""

  [ $i -eq 6 ] && ltype="$ltype lt 7"
 
  echo -n ", '' u $i $ltype w l"
done
echo
} | gnuplot -persist



