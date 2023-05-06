CC=g++
SRC=src
BIN=bin
OBJ=obj
ARGS=-g
TEST=tests
DB=gdb
FILE=$(TEST)/test.classic


include src/lexical/Makefile
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

clean:
	rm -rf obj/* bin/* src/lexical/lex.yy.c src/lexical/lex.h
