lexer: $(BIN)/lexer

$(BIN)/lexer: $(OBJ)/scan.o $(OBJ)/lex.yy.o $(OBJ)/util.o $(OBJ)/tokens.o
	$(CC) $(ARGS) $^ -o $@

$(SRC)/lexical/lex.yy.c: $(SRC)/lexical/classic.lex
	lex -v -o $@ $<

$(OBJ)/%.o: $(SRC)/lexical/%.cpp
	$(CC) $(ARGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/lexical/%.c
	$(CC) $(ARGS) -c $< -o $@
