all: dsa172a1

DEFS = -DUSE_SOME_DEF

dsa172a1: dbLib.o requestLib.o main.o processData.o
	g++ main.o dbLib.o requestLib.o processData.o -std=c++11 -o dsa172a1 $(DEFS)
main.o: main.cpp listLib.h dbLib.h
	g++ -c main.cpp -std=c++11 $(DEFS)
requestLib.o: requestLib.cpp requestLib.h listLib.h
	g++ -c requestLib.cpp -std=c++11 $(DEFS)
dbLib.o: dbLib.cpp dbLib.h listLib.h requestLib.h
	g++ -c dbLib.cpp -std=c++11 $(DEFS)
processData.o: processData.cpp listLib.h requestLib.h dbLib.h
	g++ -c processData.cpp -std=c++11 $(DEFS)
	
debug: testlist.o
	g++ testlist.o -o debug $(DEFS)

testlist.o: testlist.cpp listLib.h
	g++ -c testlist.cpp listLib.h $(DEFS) 

clean:
	rm *.o dsa172a1 debug
