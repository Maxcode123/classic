
test-tokens: $(BIN)/test_tokens
	$<

$(BIN)/test_tokens: $(OBJ)/tokens.o $(OBJ)/test_tokens.o
	$(CC) $^ -o $@ -lgtest -lgtest_main

$(OBJ)/%.o: $(TEST)/units/%.cpp
	$(CC) $(ARGS) -c $< -o $@
