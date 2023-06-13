socialNetwork: main.o Tokenizer.o User.o ; g++ -o socialNetwork Tokenizer.o User.o main.o 


main.o: main.cpp Tokenizer.hpp User.cpp ; g++ -c main.cpp


Tokenizer.o: Tokenizer.cpp Tokenizer.hpp ; g++ -c Tokenizer.cpp


User.o: User.cpp User.hpp ; g++ -c User.cpp


clean: ; rm socialNetwork main.o Tokenizer.o User.o *.html
