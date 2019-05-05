#!/bin/bash
echo "lancement du script"
if [[ "$#" -lt 4 && "$#" -gt 5 ]]
then
  echo "Utilisation : ./lancement.sh ipServeur portServeur ipIa portIa name?"
  exit 1
fi
NAME="JENKINS"
if [ "$#" -eq 5 ]
then
  NAME="$5"
fi
echo "Compilation"

make

java -cp .:IA/:IA/jasper.jar jSicstus "$4" &

sleep 1

echo "Lancement du client"

./client "$1" "$2" "$NAME" "$4" "$3"
#java "-classpath /usr/local/sicstus4.4.1/lib/sicstus-4.4.1/bin/jasper.jar:. ./IA/jSicstus $4"
