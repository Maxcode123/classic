parser: $(BIN)/parser

$(BIN)/parser: $(OBJ)/classical.tab.o $(OBJ)/lex.yy.o $(OBJ)/tokens.o $(OBJ)/parse.o
	$(CC) $(ARGS) $^ -o $@

bison: $(SRC)/syntax/classical.tab.c

$(SRC)/syntax/classical.tab.c: $(SRC)/syntax/classical.y
	bison $< -H -Wcounterexamples -o $@

$(OBJ)/%.o: $(SRC)/syntax/%.cpp
	$(CC) $(ARGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/syntax/%.c
	$(CC) $(ARGS) -c $< -o $@

clean-syntax:
	$(RM) src/syntax/classical.tab.c
