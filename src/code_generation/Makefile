LLVM_LIB=-lLLVM-15
PARSER_OBJ=$(OBJ)/classical.tab.o $(OBJ)/lex.yy.o $(OBJ)/tokens.o $(OBJ)/parse.o $(OBJ)/nodes.o $(OBJ)/builder.o

.PHONY: generator
generator: $(BIN)/generator

.PHONY: run-ir-builder
run-ir-builder: $(BIN)/ir_builder
	$<

$(BIN)/generator: $(PARSER_OBJ) $(OBJ)/code_generator.o $(OBJ)/generation_main.o
	$(CC) $(ARGS) $^ -o $@ $(LLVM_LIB)

$(BIN)/ir_builder: $(OBJ)/ir_builder.o $(OBJ)/nodes.o
	$(CC) $(ARGS) $^ -o $@ $(LLVM_LIB)

$(OBJ)/%.o: $(SRC)/code_generation/%.cpp
	$(CC) $(ARGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/code_generation/%.c
	$(CC) $(ARGS) -c $< -o $@
