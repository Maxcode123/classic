lexer: $(BIN)/lexer

$(BIN)/lexer: $(OBJ)/scan.o $(OBJ)/lex.yy.o $(OBJ)/util.o $(OBJ)/hashtable.o
	$(CC) $(ARGS) $^ -o $@

$(SRC)/lexical/lex.yy.c: $(SRC)/lexical/classic.lex
	lex -o $@ $<

$(OBJ)/%.o: $(SRC)/lexical/%.c
	$(CC) $(ARGS) -c $< -o $@
