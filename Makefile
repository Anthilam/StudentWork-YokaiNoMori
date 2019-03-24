LD_FLAGS =

all: IA Client Serveur

IA:
	javac -d ./ ./IA/*.java

Client: ./Client/client.c ./TCP/fonctionsTCP.o libClient.o ./TCP/protocolYokai.h
	gcc -Wall ./Client/client.c -o client ./TCP/fonctionsTCP.o ./Client/libClient.o $(LD_FLAGS)

fonctionsTCP.o: ./TCP/fonctionsTCP.c ./TCP/fonctionsTCP.h
	gcc -W -Wall -g -std=c99 -o $@ -c $<

libClient.o: ./Client/libClient.c ./Client/libClient.h ./TCP/protocolYokai.h
	gcc -W -Wall -g -std=c99 -o ./Client/$@ -c $<

Serveur: ./Serveur/serveur.c ./TCP/fonctionsTCP.o ./Serveur/libServeur.o ./TCP/protocolYokai.h
	gcc -Wall ./Serveur/serveur.c -o serveur  ./TCP/fonctionsTCP.o ./Serveur/libServeur.o $(LD_FLAGS)

libServeur.o: ./Serveur/libServeur.c ./Serveur/libServeur.h ./TCP/protocolYokai.h
	gcc -W -Wall -g -std=c99 -o ./Serveur/$@ -c $<

clean:
	rm -f ./TCP/fonctionsTCP.o;
	rm -f ./Client/*.o
	rm -f ./Serveur/*.o

mrproper: clean
	rm -f client;
	rm -f serveur;