#!/bin/sh
echo "comparaison des programmes"
cp Archipelago ../Comparaison/
cd ../Comparaison
if [ $# != 0 ]
then
  echo "---test sur e$1 -----------------------------------------"
  echo " "
  ./demo console e$1.txt
  ./Archipelago e$1.txt
  echo " "
  echo "--- fin du test -----------------------------------------"
  echo " "
else
  for i in 01 02 03 04 05 06 07 08 09 10
  do
    echo "---test sur e$i -----------------------------------------"
    echo " "
    ./demo console e$i.txt
    ./Archipelago e$i.txt
    echo " "
    echo "--- fin du test -----------------------------------------"
    echo " "
  done
fi
