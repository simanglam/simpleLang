all: slc slt slp

.PHONY: all

slc: compiler.o parser.o asts.o tokenizer.o
	g++ compiler.o parser.o asts.o tokenizer.o -o slc $(shell llvm-config  --ldflags --libs)

slt: tokenizerTest.o parser.o asts.o tokenizer.o
	g++ tokenizerTest.o parser.o asts.o tokenizer.o -o slt $(shell llvm-config  --ldflags --libs)

slp: parserTest.o parser.o asts.o tokenizer.o
	g++ parserTest.o parser.o asts.o tokenizer.o -o slp $(shell llvm-config  --ldflags --libs)

compiler.o: compiler.h compiler.cpp
	g++ compiler.cpp -c $(shell llvm-config --system-libs core support --cxxflags)

parser.o: parser.h parser.cpp
	g++ parser.cpp -c $(shell llvm-config --system-libs core support --cxxflags)

asts.o: asts.h asts.cpp
	g++ asts.cpp -c $(shell llvm-config --system-libs core support --cxxflags)

tokenizer.o: tokenizer.h tokenizer.cpp
	g++ tokenizer.cpp -c $(shell llvm-config --system-libs core support --cxxflags)

tokenizerTest.o: tokenizerTest.cpp
	g++ tokenizerTest.cpp -c $(shell llvm-config --system-libs core support --cxxflags)

parserTest.o: parserTest.cpp
	g++ parserTest.cpp -c $(shell llvm-config --system-libs core support --cxxflags)