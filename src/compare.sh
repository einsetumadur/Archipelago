#!/bin/sh
echo "comparaison des programmes"
cp Archipelago ../Comparaison/
cd ../Comparaison
if [ $# != 0 ]
then
  if [ $1 == "-t" ]
  then

    rm demo_output.txt prog_output.txt

    for i in 01 02 03 04 05 06 07 08 09 10 
    do
      ./demo console e$i.txt >> demo_output.txt
      ./Archipelago e$i.txt >> prog_output.txt
    done

    if cmp -s demo_output.txt prog_output.txt
    then
    printf "same output, Boulic is proud ! \n"
    else
    printf 'The file "%s" is different from "%s"\n' demo_output.txt prog_output.txt
    diff demo_output.txt prog_output.txt
    fi

  else
    echo "---test sur e$1 -----------------------------------------"
    echo " "
    ./demo console e$1.txt
    ./Archipelago e$1.txt
    echo " "
    echo "--- fin du test -----------------------------------------"
    echo " "
  fi

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
