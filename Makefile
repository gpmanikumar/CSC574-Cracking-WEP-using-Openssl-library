all: crack.cpp
	g++ -o crack crack.cpp
clean:
	$(RM) crack
