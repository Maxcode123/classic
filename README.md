# classic - _classic_ - _κλασσική_
A computer language inspired by _classical_ studies (ancient Greek and Latin
 literature).  
 Keywords are derived from Latin and Greek, e.g. the usual `return` word
 is `exodus` (from Greek _ἔξοδος_ = exit) in classic; a function is declared with the keyword `oper` (from _operatio_ = operation).  
```
oper: int simple_function(int myint) {
	exodus myint;
}

oper: int initus() {
	exodus simple_function(myint=0);
}
```

`initus` is the main function that gets executed.
 
## Grammar - _Grammatica_ - _Γραμματική_

### Terminals - _Terminales_ - _Τερματικά_

**ID** = [a-zA-Z_][a-zA-Z0-9_]*  
**INT** = [-]?[0-9]+  
**DUPL** = [-+]?[0-9]+\.?[0-9]*  

### Non-terminals - _Non-terminales_ - _Ἀτέρμονα_   
1. **program** = **classdefs** ,  **funcs**  
2. **classdefs** = **classdef-list**  
3. **classdef-list** = [ **non-empty-classdef-list** ]  
4. **non-empty-classdef-list** = **classdef** , { "," , **classdef** }  
5. **classdef** = 'exemp' , ";" , **custom-type** , "{" , **class-body** , "}"  
6. **custom-type** = **ID**  
7. **class-body** = **attr-decl-list** | **attr-decl-list** , **func-list**  
8. **attr-decl-list** = [ **non-empty-attr-decl-list** ]  
9. **non-empty-attr-decl-list** = **attr-decl** , { "," , **attr-decl** }  
10. **attr-decl** = **type** , **ID** , ";"  
11. **type** = 'int' | 'dupl' | 'str' | **custom-type**  
12. **funcs** = **initus-func** | **func-list** , **initus-func**  
13. **initus-func** = 'oper' , ":" , 'int' , 'initus' , "(" , ")" , "{" **func-body** , "}"  
14. **func-list** = **func** , { "," , **func** }    
15. **func** = 'oper' , ":" , **exodus-type** ,  **func-id** , "(" , **param-list** , ")" , "{" , **func-body** , "}"  
16. **func-id** = **ID**  
17. **exodus-type** = 'anef' | **type**  
18. **param-list** = [ **non-empty-param-list** ]  
19. **non-empty-param-list** = **param** , { "," , **param** }   
20. **param** = **type** , **param-id**  
21. **param-id** = **ID**  
22. **func-body** = **statement** , **exodus-stm**  
23. **exodus-stm** = 'exodus' , **exp** , ";"  
24. **statement** = [ **non-empty-stm** ]  
25. **non-empty-stm** = **compound-stm** | **assign-stm** | **exp**  
26. **compound-stm** = **assign-or-exp-stm** , ";" , **assign-or-exp-stm** , ";" | **compound-stm** , **assign-or-exp-stm** , ";"  
27. **assign-or-exp-stm** = **assign-stm** | **exp**    
28. **assign-stm** = **ids** , "=" , **exp**  
29. **ids** = **ID**  
30. **exp** = "(" , **exp** , ")" | **binop-exp** | **func-call** | **literal** | **variable**  
31. **binop-exp** = **exp** , "+" , **exp** | **exp** , "-" , **exp** | **exp** , "/" , **exp** | **exp** , "*" , **exp**  
32. **func-call** = **ids** , "(" , **arg-list** , ")"  
33. **arg-list** = [ **non-empty-arg-list** ]  
34. **non-empty-arg-list** = **arg** , { "," , **arg** }   
35. **arg** = **arg-id** , "=" , **exp**  
36. **literal** = **INT** | **DUPL** | 'anef'  
37. **variable** = **ID**    
</br>

# Current state - _Current status_ - _Τρέχουσα κατάστασις_
 * lexical analysis ✅
 * syntax analysis ✅
 * semantic analysis
 * intermediate representation
 * code production

# Requirements - _Necessaria_ - _Προαπαιτούμενα_ 
* [g++](https://gcc.gnu.org/)
* [Flex](https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/flex.html)
* [Bison](https://www.gnu.org/software/bison/)
* [make](https://www.gnu.org/software/make/)
* [Google test](http://google.github.io/googletest/) (only for testing)

# Usage - _Usus_ - _Χρήσις_
If you meet the requirements you can download and install the _classic_ compiler and use
it to write your own little programs.
```
$ git clone https://github.com/Maxcode123/classic.git  && \
 cd classic && \
 make && \
 make test && \
 sudo make install  
```

# Documentation - _Documentum_ - _Ἐγχειρίδιον_ 
 **All commands mentioned should be executed from the root directory.**

## Lexer
The lexer is documented in [src/lexical](https://github.com/Maxcode123/classic/tree/syntax/src/lexical)

## Parser
The parser is documented in [src/syntax](https://github.com/Maxcode123/classic/tree/syntax/src/syntax).


## Tests
[Google test](http://google.github.io/googletest/) library is used for testing.  
All tests are contained in the `tests` directory.  
Run unit tests with `make unit-test`.  
Run integration tests with `make integration-test`.
