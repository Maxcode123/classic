TESTLIB=-lgtest -lgtest_main
CCTEST=$(CC) $^ -o $@ $(TESTLIB)

.PHONY: run-main
run-main: $(BIN)/test_main
	$<

.PHONY: ir
ir: $(TEST)/test.ll

$(TEST)/test.ll: $(TEST)/test.c
	clang -emit-llvm -S $< -o $@

.PHONY: integration-test
integration-test: integration-test-parser

.PHONY: integration-test-parser
integration-test-parser: $(BIN)/test_parser
	$<

$(BIN)/test_parser: $(OBJ)/test_parser.o $(OBJ)/classical.tab.o $(OBJ)/mock_lex.yy.o $(OBJ)/tokens.o $(OBJ)/builder.o $(OBJ)/nodes.o $(OBJ)/parse.o
	$(CCTEST)


.PHONY: unit-test
unit-test: unit-test-parser unit-test-scanner unit-test-utils unit-test-generator

.PHONY: unit-test-generator
unit-test-generator: test-code-generator

.PHONY: unit-test-parser
unit-test-parser: test-nodes test-builder

.PHONY: unit-test-scanner
unit-test-scanner: test-tokens

.PHONY: unit-test-utils
unit-test-utils: test-error-handler

.PHONY: test-code-generator
test-code-generator: $(BIN)/test_code_generator
	$<

.PHONY: test-builder
test-builder: $(BIN)/test_builder
	$<

.PHONY: test-nodes
test-nodes: $(BIN)/test_nodes
	$<

.PHONY: test-error-handler
test-error-handler: $(BIN)/test_error_handler
	$<

.PHONY: test-tokens
test-tokens: $(BIN)/test_tokens
	$<

$(BIN)/test_code_generator: $(OBJ)/nodes.o $(OBJ)/code_generator.o $(OBJ)/test_code_generator.o
	$(CCTEST) -lLLVM-15

$(BIN)/test_builder: $(OBJ)/builder.o $(OBJ)/nodes.o $(OBJ)/test_builder.o
	$(CCTEST)

$(BIN)/test_nodes: $(OBJ)/nodes.o $(OBJ)/test_nodes.o
	$(CCTEST)

$(BIN)/test_error_handler: $(OBJ)/test_error_handler.o
	$(CCTEST)

$(BIN)/test_tokens: $(OBJ)/tokens.o $(OBJ)/test_tokens.o
	$(CCTEST)


$(OBJ)/%.o: $(TEST)/units/%.cpp
	$(CC) $(ARGS) -c $< -o $@

$(OBJ)/%.o: $(TEST)/units/code_generation/%.cpp
	$(CC) $(ARGS) -c $< -o $@

$(OBJ)/%.o: $(TEST)/units/syntax/ast/%.cpp
	$(CC) $(ARGS) -c $< -o $@

$(OBJ)/%.o: $(TEST)/units/lexical/%.cpp
	$(CC) $(ARGS) -c $< -o $@

$(OBJ)/%.o: $(TEST)/units/utils/%.cpp
	$(CC) $(ARGS) -c $< -o $@

$(OBJ)/%.o: $(TEST)/integration/syntax/%.cpp
	$(CC) $(ARGS) -c $< -o $@

$(BIN)/test_main: $(OBJ)/nodes.o $(OBJ)/builder.o $(TEST)/test_main.cpp
	$(CC) $(ARGS) $^ -o $@
