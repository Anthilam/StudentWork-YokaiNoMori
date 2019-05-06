#!/bin/bash

if [ "$#" -lt 3 ]
then
  echo "USAGE : ./joueur.sh ipServeur portServeur nom [ portIA sicstuslibpath ]"
  exit 1
fi

# Vars
PORT=55555
LOCALHOST="127.0.0.1"
SICSTUSLIB="./IA/libSicstus4.4.1"

if [ "$#" -eq 4 ]
then
  PORT=$4
fi

if [ "$#" -eq 5 ]
then
  PORT=$4
  SICSTUSLIB=$5
fi

# Add -hold option to xterm to keep the terminal open

# Compilation
echo "Compilation du joueur"
make client

# Start AI
echo "Lancement de l'IA"
java -Djava.library.path=$SICSTUSLIB -cp .:IA/:./IA/libSicstus4.4.1/sicstus-4.4.1/bin/jasper.jar jSicstus $PORT &

sleep 1

# Start client
echo "Lancement du client"
./client "$1" "$2" "$3" "$PORT" "$LOCALHOST"
