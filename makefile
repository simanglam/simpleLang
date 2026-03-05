slc: compiler.o parser.o asts.o tokenizer.o
	g++ compiler.o parser.o asts.o tokenizer.o -o slc $(shell llvm-config  --ldflags --libs)

compiler.o: compiler.h compiler.cpp
	g++ compiler.cpp -c $(shell llvm-config --system-libs core support --cxxflags)

parser.o: parser.h parser.cpp
	g++ parser.cpp -c $(shell llvm-config --system-libs core support --cxxflags)

asts.o: asts.h asts.cpp
	g++ asts.cpp -c $(shell llvm-config --system-libs core support --cxxflags)

tokenizer.o: tokenizer.h tokenizer.cpp
	g++ tokenizer.cpp -c $(shell llvm-config --system-libs core support --cxxflags)