.PHONY: parser
parser: $(BIN)/parser

$(BIN)/parser: $(OBJ)/classical.tab.o $(OBJ)/lex.yy.o $(OBJ)/tokens.o $(OBJ)/parse_main.o $(OBJ)/parse.o $(OBJ)/nodes.o $(OBJ)/builder.o
	$(CC) $(ARGS) $^ -o $@

.PHONY: bison
bison: $(SRC)/syntax/classical.tab.c

$(SRC)/syntax/classical.tab.c: $(SRC)/syntax/classical.y
	bison $< -H -Wcounterexamples -o $@ --report=states,itemsets --report-file=$(SRC)/syntax/states.bison

$(OBJ)/%.o: $(SRC)/syntax/%.cpp
	$(CC) $(ARGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/syntax/ast/%.cpp
	$(CC) $(ARGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/syntax/%.c
	$(CC) $(ARGS) -c $< -o $@

.PHONY: clean-syntax
clean-syntax:
	$(RM) src/syntax/classical.tab.c
