BOOSTDIR='/usr/include'
Leda = '/usr/local/LEDA/incl'
Ledalib = '/usr/local/LEDA'
LIBFLAGS= -lleda -lm

f=Algo.cpp

compile: $(f)
	g++ $(f) -o algo -O3 -std=c++0x -fno-strict-aliasing -I $(BOOSTDIR) -I $(Leda) -L $(Ledalib) $(LIBFLAGS)
clean:
	rm -f algo
run:
	./algo
