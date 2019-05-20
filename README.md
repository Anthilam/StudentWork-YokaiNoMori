# Yokai No-Mori project

The Yokai No-Mori project goal was to create an AI in Prolog that can play the Yokai No-Mori game.

This project is composed of :
* a game-server : written in C, that links the two players and validate each move
* a client, made of :
  1. A C program that communicates with the game-server
  2. A Java program that uses the *Jasper* library provided by *Sicstus Prolog* to use our Prolog predicates defined in `ia.pl`
  3. Prolog predicates written in the `ia.pl` file

## Build

Compile with `make` command, you have options to compile only the client, the server or the AI

## Launch

Launch the server by using `./serveur nPort` (or use `./yokaiServeur nPort`)

Launch a client by using the script `./joueur.sh server_ip server_port player_name [ AI_port sicstuslibpath ]`
