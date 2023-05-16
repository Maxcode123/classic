CC=g++
SRC=src
BIN=bin
OBJ=obj
ARGS=-g
TEST=tests
DB=gdb
FILE=$(TEST)/test.classic
RM=rm -rf


include src/lexical/Makefile
include src/syntax/Makefile
include tests/Makefile


run-lexer: $(BIN)/lexer
	$< $(FILE)

debug-file: $(TEST)/bin/main
	clear
	$(DB) --args $< $(FILE)

debug: $(TEST)/bin/main
	clear
	$(DB) $<

run-file: $(TEST)/bin/main
	clear
	$< $(FILE)

run: $(TEST)/bin/main
	clear
	$<

$(OBJ)/%.o: $(SRC)/utils/%.c
	$(CC) $(ARGS) -c $< -o $@

clean: clean-binaries clean-object clean-lexical clean-syntax 

clean-binaries:
	$(RM) bin/*

clean-object:
	$(RM) obj/*
