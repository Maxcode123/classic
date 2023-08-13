.PHONY: lexer
lexer: $(BIN)/lexer

$(BIN)/lexer: $(OBJ)/lex_main.o $(OBJ)/lex.yy.o $(OBJ)/tokens.o
	$(CC) $(ARGS) $^ -o $@

.PHONY: flex
flex: $(SRC)/lexical/lex.yy.c

$(SRC)/lexical/lex.yy.c: $(SRC)/lexical/classic.lex
	lex -v -o $@ $<

$(OBJ)/%.o: $(SRC)/lexical/%.cpp
	$(CC) $(ARGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/lexical/%.c
	$(CC) $(ARGS) -c $< -o $@

.PHONY: clean-lexical
clean-lexical:
	$(RM) src/lexical/lex.yy.c src/lexical/lex.h
