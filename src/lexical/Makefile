lexer: $(SRC)/lexical/lexer

$(SRC)/lexical/lexer: $(OBJ)/lexdriver.o $(OBJ)/lex.yy.o $(OBJ)/util.o
	$(CC) $(ARGS) $^ -o $@

$(OBJ)/lexdriver.o: $(SRC)/scanner/lexdriver.c
	$(CC) $(ARGS) -c $< -o $@

$(SRC)/lexical/lex.yy.c: $(SRC)/lexical/classic.lex
	lex -o $@ $<

$(OBJ)/%.o: $(SRC)/lexical/%.c
	$(CC) $(ARGS) -c $< -o $@
