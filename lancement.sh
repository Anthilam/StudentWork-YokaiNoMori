#!/bin/bash
echo "lancement du script"
if [ "$#" -lt 3 ]
then
  echo "Utilisation : ./lancement.sh ipServeur portServeur nom"
  exit 1
fi
echo "Compilation"
PORT=55555
LOCALHOST=127.0.0.1
if [ "$#" -eq 4 ]
then
  PORT=$4
fi



make
echo $1
echo $2
echo $3
echo $PORT
java -Djava.library.path="/usr/local/sicstus4.4.1/lib" -cp .:IA/:IA/jasper.jar jSicstus $PORT &

sleep 1

echo "Lancement du client"

./client "$1" "$2" "$3" "$PORT"  "$LOCALHOST"
#java "-classpath /usr/local/sicstus4.4.1/lib/sicstus-4.4.1/bin/jasper.jar:. ./IA/jSicstus $4"
