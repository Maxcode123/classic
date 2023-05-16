
parser: $(SRC)/syntax/classical.tab.c

$(SRC)/syntax/classical.tab.c: $(SRC)/syntax/classical.y
	bison $< -H -Wcounterexamples -o $@

clean-syntax:
	$(RM) src/syntax/classical.tab.c src/syntax/classical.tab.h src/syntax/classical.output
