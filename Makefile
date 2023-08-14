CC=g++
SRC=src
BIN=bin
OBJ=obj
ARGS=-g -Wno-write-strings -Werror
TEST=tests
DB=gdb
FILE=$(TEST)/test.classic
RM=rm -rf


include src/lexical/Makefile
include src/syntax/Makefile
include src/code_generation/Makefile
include tests/Makefile

.DEFAULT_GOAL := all

.PHONY: all
all: lexer parser

.PHONY: all
install:
	cp bin/parser /usr/bin/classic

.PHONY: test
test: unit-test integration-test

.PHONY: run-lexer
run-lexer: $(BIN)/lexer
	$< $(FILE)

.PHONY: run-parser
run-parser: $(BIN)/parser
	$< $(FILE)

.PHONY: run-generator
run-generator: $(BIN)/generator
	$< $(FILE)

$(OBJ)/%.o: $(SRC)/utils/%.c
	$(CC) $(ARGS) -c $< -o $@

.PHONY: clean
clean: clean-binaries clean-object clean-lexical clean-syntax

.PHONY:clean-binaries
clean-binaries:
	$(RM) bin/*

.PHONY:clean-object
clean-object:
	$(RM) obj/*
