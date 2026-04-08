all: tester.o

tester.o: tester.cpp tinyekf.hpp
	g++ -Wall -c tester.cpp

tinyekf.hpp: ekfgen.py
	./ekfgen.py 7 > tinyekf.hpp

dump:
	./ekfgen.py 7 | less

clean:
	rm -f tester.o tinyekf.hpp
