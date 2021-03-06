################################################################################
#  MAKEFILE
################################################################################

JAVA = ./IA/
JASPER = ./IA/libSicstus4.4.1/sicstus-4.4.1/bin/jasper.jar

JC = javac

JFLAGS = -g -sourcepath $(JAVA) -classpath $(JASPER):.

JSRC = \
	$(JAVA)jSicstus.java \
	$(JAVA)Coup.java \
	$(JAVA)Prolog.java

classes = $(JSRC:.java=.class)

# Java classes
%.class : %.java
	$(JC) $(JFLAGS) $^

################################################################################
#  main commands
################################################################################

all: ia client server

ia: $(classes)

client: ClientC ia

server: Serveur

################################################################################
#  Sub-commands
################################################################################

ClientC: ./Client/client.c ./TCP/fonctionsTCP.o libClient.o ./TCP/protocolYokai.h ./Client/protocoleIa.h
	gcc -Wall ./Client/client.c -o client ./TCP/fonctionsTCP.o ./Client/libClient.o $(LD_FLAGS)

fonctionsTCP.o: ./TCP/fonctionsTCP.c ./TCP/fonctionsTCP.h
	gcc -W -Wall -g -std=c99 -o $@ -c $<

libClient.o: ./Client/libClient.c ./Client/libClient.h ./TCP/protocolYokai.h
	gcc -W -Wall -g -std=c99 -o ./Client/$@ -c $<

Serveur: ./Serveur/serveur.c ./TCP/fonctionsTCP.o ./Serveur/libServeur.o ./Serveur/yokai-fPIC.o ./TCP/protocolYokai.h ./Serveur/validation.h
	gcc -Wall ./Serveur/serveur.c -o serveur  ./TCP/fonctionsTCP.o ./Serveur/libServeur.o ./Serveur/yokai-fPIC.o $(LD_FLAGS)

libServeur.o: ./Serveur/libServeur.c ./Serveur/libServeur.h ./TCP/protocolYokai.h
	gcc -W -Wall -g -std=c99 -o ./Serveur/$@ -c $<

################################################################################
#  Cleaning commands
################################################################################

clean:
	rm -f ./TCP/fonctionsTCP.o;
	rm -f ./Client/*.o
	rm -f ./Serveur/libServeur.o
	rm -f ./Shared/*.o
	rm -f ./IA/*.class

mrproper: clean
	rm -f client;
	rm -f serveur;
