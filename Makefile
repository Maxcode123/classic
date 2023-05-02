CC=gcc
SRC=src
OBJ=obj
ARGS=-g
TEST=tests
DB=gdb
FILE=test.cls


include src/lexical/Makefile


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
	rm -rf obj/*
