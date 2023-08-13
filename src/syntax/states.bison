Terminals unused in grammar

    "ego"
    "+"
    "-"
    "/"
    "*"
    "."


Grammar

    0 $accept: program $end

    1 program: classdefs funcs

    2 classdefs: classdef-list

    3 classdef-list: ε
    4              | non-empty-classdef-list

    5 non-empty-classdef-list: classdef
    6                        | non-empty-classdef-list classdef

    7 classdef: "exemp" ":" custom-type "{" class-body "}"

    8 custom-type: _ID

    9 class-body: attr-decl-list
   10           | attr-decl-list func-list

   11 attr-decl-list: ε
   12               | non-empty-attr-decl-list

   13 non-empty-attr-decl-list: attr-decl
   14                         | non-empty-attr-decl-list attr-decl

   15 attr-decl: type _ID ";"

   16 type: "int"
   17     | "dupl"
   18     | "str"
   19     | custom-type

   20 funcs: initus-func
   21      | func-list initus-func

   22 initus-func: "oper" ":" "int" "initus" "(" ")" "{" func-body "}"

   23 func-list: func
   24          | func-list func

   25 func: "oper" ":" exodus-type func-id "(" param-list ")" "{" func-body "}"

   26 func-id: _ID

   27 exodus-type: "anef"
   28            | type

   29 param-list: ε
   30           | non-empty-param-list

   31 non-empty-param-list: param
   32                     | non-empty-param-list "," param

   33 param: type param-id

   34 param-id: _ID

   35 func-body: statement exodus-stm

   36 exodus-stm: "exodus" exp ";"

   37 statement: empty-stm
   38          | non-empty-stm

   39 non-empty-stm: compound-stm
   40              | assign-stm
   41              | exp

   42 empty-stm: ε

   43 compound-stm: assign-or-exp-stm ";" assign-or-exp-stm ";"
   44             | compound-stm assign-or-exp-stm ";"

   45 assign-or-exp-stm: assign-stm
   46                  | exp

   47 assign-stm: ids "=" exp

   48 ids: _ID

   49 exp: "(" exp ")"
   50    | binop-exp
   51    | func-call
   52    | literal
   53    | variable

   54 binop-exp: exp binop exp

   55 binop: _PLUS
   56      | _MINUS
   57      | _DIV
   58      | _TIMES

   59 func-call: ids "(" arg-list ")"

   60 arg-list: ε
   61         | non-empty-arg-list

   62 non-empty-arg-list: arg
   63                   | non-empty-arg-list "," arg

   64 arg: arg-id "=" exp

   65 arg-id: _ID

   66 literal: _INT
   67        | _DUPL
   68        | _STR
   69        | "anef"

   70 variable: _ID


Terminals, with rules where they appear

    $end (0) 0
    error (256)
    "oper" <sval> (258) 22 25
    "exemp" <sval> (259) 7
    "int" <sval> (260) 16 22
    "dupl" <sval> (261) 17
    "str" <sval> (262) 18
    "anef" <sval> (263) 27 69
    "ego" <sval> (264)
    "initus" <sval> (265) 22
    "exodus" <sval> (266) 36
    _PLUS <sval> (267) 55
    "+" <sval> (268)
    _MINUS <sval> (269) 56
    "-" <sval> (270)
    _DIV <sval> (271) 57
    "/" <sval> (272)
    _TIMES <sval> (273) 58
    "*" <sval> (274)
    "(" <sval> (275) 22 25 49 59
    ")" <sval> (276) 22 25 49 59
    "{" <sval> (277) 7 22 25
    "}" <sval> (278) 7 22 25
    ":" <sval> (279) 7 22 25
    ";" <sval> (280) 15 36 43 44
    "." <sval> (281)
    "," <sval> (282) 32 63
    "=" <sval> (283) 47 64
    _ID <sval> (284) 8 15 26 34 48 65 70
    _INT <ival> (285) 66
    _DUPL <dval> (286) 67
    _STR <sval> (287) 68
    BINOP (288)


Nonterminals, with rules where they appear

    $accept (34)
        on left: 0
    program <prog> (35)
        on left: 1
        on right: 0
    classdefs <classdef_list> (36)
        on left: 2
        on right: 1
    classdef-list <classdef_list> (37)
        on left: 3 4
        on right: 2
    non-empty-classdef-list (38)
        on left: 5 6
        on right: 4 6
    classdef <clsdef> (39)
        on left: 7
        on right: 5 6
    custom-type <sval> (40)
        on left: 8
        on right: 7 19
    class-body (41)
        on left: 9 10
        on right: 7
    attr-decl-list (42)
        on left: 11 12
        on right: 9 10
    non-empty-attr-decl-list (43)
        on left: 13 14
        on right: 12 14
    attr-decl (44)
        on left: 15
        on right: 13 14
    type <sval> (45)
        on left: 16 17 18 19
        on right: 15 28 33
    funcs <func_list> (46)
        on left: 20 21
        on right: 1
    initus-func <function> (47)
        on left: 22
        on right: 20 21
    func-list <func_list> (48)
        on left: 23 24
        on right: 10 21 24
    func <function> (49)
        on left: 25
        on right: 23 24
    func-id <sval> (50)
        on left: 26
        on right: 25
    exodus-type <sval> (51)
        on left: 27 28
        on right: 25
    param-list <param_list> (52)
        on left: 29 30
        on right: 25
    non-empty-param-list <param_list> (53)
        on left: 31 32
        on right: 30 32
    param <param_t> (54)
        on left: 33
        on right: 31 32
    param-id <sval> (55)
        on left: 34
        on right: 33
    func-body <func_body> (56)
        on left: 35
        on right: 22 25
    exodus-stm <exodus_stm> (57)
        on left: 36
        on right: 35
    statement <stm_t> (58)
        on left: 37 38
        on right: 35
    non-empty-stm <stm_t> (59)
        on left: 39 40 41
        on right: 38
    empty-stm <stm_t> (60)
        on left: 42
        on right: 37
    compound-stm <cmp_stm_t> (61)
        on left: 43 44
        on right: 39 44
    assign-or-exp-stm (62)
        on left: 45 46
        on right: 43 44
    assign-stm <asgn_stm_t> (63)
        on left: 47
        on right: 40 45
    ids <sval> (64)
        on left: 48
        on right: 47 59
    exp <exp_t> (65)
        on left: 49 50 51 52 53
        on right: 36 41 46 47 49 54 64
    binop-exp <binop_exp> (66)
        on left: 54
        on right: 50
    binop (67)
        on left: 55 56 57 58
        on right: 54
    func-call <funcall_exp> (68)
        on left: 59
        on right: 51
    arg-list <arg_list> (69)
        on left: 60 61
        on right: 59
    non-empty-arg-list <arg_list> (70)
        on left: 62 63
        on right: 61 63
    arg <arg_t> (71)
        on left: 64
        on right: 62 63
    arg-id <sval> (72)
        on left: 65
        on right: 64
    literal <ltr_exp> (73)
        on left: 66 67 68 69
        on right: 52
    variable <var_exp> (74)
        on left: 70
        on right: 53


State 0

    0 $accept: • program $end
    1 program: • classdefs funcs
    2 classdefs: • classdef-list
    3 classdef-list: ε •
    4              | • non-empty-classdef-list
    5 non-empty-classdef-list: • classdef
    6                        | • non-empty-classdef-list classdef
    7 classdef: • "exemp" ":" custom-type "{" class-body "}"

    "exemp"  shift, and go to state 1

    $default  reduce using rule 3 (classdef-list)

    program                  go to state 2
    classdefs                go to state 3
    classdef-list            go to state 4
    non-empty-classdef-list  go to state 5
    classdef                 go to state 6


State 1

    7 classdef: "exemp" • ":" custom-type "{" class-body "}"

    ":"  shift, and go to state 7


State 2

    0 $accept: program • $end

    $end  shift, and go to state 8


State 3

    1 program: classdefs • funcs
   20 funcs: • initus-func
   21      | • func-list initus-func
   22 initus-func: • "oper" ":" "int" "initus" "(" ")" "{" func-body "}"
   23 func-list: • func
   24          | • func-list func
   25 func: • "oper" ":" exodus-type func-id "(" param-list ")" "{" func-body "}"

    "oper"  shift, and go to state 9

    funcs        go to state 10
    initus-func  go to state 11
    func-list    go to state 12
    func         go to state 13


State 4

    2 classdefs: classdef-list •

    $default  reduce using rule 2 (classdefs)


State 5

    4 classdef-list: non-empty-classdef-list •
    6 non-empty-classdef-list: non-empty-classdef-list • classdef
    7 classdef: • "exemp" ":" custom-type "{" class-body "}"

    "exemp"  shift, and go to state 1

    $default  reduce using rule 4 (classdef-list)

    classdef  go to state 14


State 6

    5 non-empty-classdef-list: classdef •

    $default  reduce using rule 5 (non-empty-classdef-list)


State 7

    7 classdef: "exemp" ":" • custom-type "{" class-body "}"
    8 custom-type: • _ID

    _ID  shift, and go to state 15

    custom-type  go to state 16


State 8

    0 $accept: program $end •

    $default  accept


State 9

   22 initus-func: "oper" • ":" "int" "initus" "(" ")" "{" func-body "}"
   25 func: "oper" • ":" exodus-type func-id "(" param-list ")" "{" func-body "}"

    ":"  shift, and go to state 17


State 10

    1 program: classdefs funcs •

    $default  reduce using rule 1 (program)


State 11

   20 funcs: initus-func •

    $default  reduce using rule 20 (funcs)


State 12

   21 funcs: func-list • initus-func
   22 initus-func: • "oper" ":" "int" "initus" "(" ")" "{" func-body "}"
   24 func-list: func-list • func
   25 func: • "oper" ":" exodus-type func-id "(" param-list ")" "{" func-body "}"

    "oper"  shift, and go to state 9

    initus-func  go to state 18
    func         go to state 19


State 13

   23 func-list: func •

    $default  reduce using rule 23 (func-list)


State 14

    6 non-empty-classdef-list: non-empty-classdef-list classdef •

    $default  reduce using rule 6 (non-empty-classdef-list)


State 15

    8 custom-type: _ID •

    $default  reduce using rule 8 (custom-type)


State 16

    7 classdef: "exemp" ":" custom-type • "{" class-body "}"

    "{"  shift, and go to state 20


State 17

    8 custom-type: • _ID
   16 type: • "int"
   17     | • "dupl"
   18     | • "str"
   19     | • custom-type
   22 initus-func: "oper" ":" • "int" "initus" "(" ")" "{" func-body "}"
   25 func: "oper" ":" • exodus-type func-id "(" param-list ")" "{" func-body "}"
   27 exodus-type: • "anef"
   28            | • type

    "int"   shift, and go to state 21
    "dupl"  shift, and go to state 22
    "str"   shift, and go to state 23
    "anef"  shift, and go to state 24
    _ID     shift, and go to state 15

    custom-type  go to state 25
    type         go to state 26
    exodus-type  go to state 27


State 18

   21 funcs: func-list initus-func •

    $default  reduce using rule 21 (funcs)


State 19

   24 func-list: func-list func •

    $default  reduce using rule 24 (func-list)


State 20

    7 classdef: "exemp" ":" custom-type "{" • class-body "}"
    8 custom-type: • _ID
    9 class-body: • attr-decl-list
   10           | • attr-decl-list func-list
   11 attr-decl-list: ε •
   12               | • non-empty-attr-decl-list
   13 non-empty-attr-decl-list: • attr-decl
   14                         | • non-empty-attr-decl-list attr-decl
   15 attr-decl: • type _ID ";"
   16 type: • "int"
   17     | • "dupl"
   18     | • "str"
   19     | • custom-type

    "int"   shift, and go to state 28
    "dupl"  shift, and go to state 22
    "str"   shift, and go to state 23
    _ID     shift, and go to state 15

    $default  reduce using rule 11 (attr-decl-list)

    custom-type               go to state 25
    class-body                go to state 29
    attr-decl-list            go to state 30
    non-empty-attr-decl-list  go to state 31
    attr-decl                 go to state 32
    type                      go to state 33


State 21

   16 type: "int" •
   22 initus-func: "oper" ":" "int" • "initus" "(" ")" "{" func-body "}"

    "initus"  shift, and go to state 34

    $default  reduce using rule 16 (type)


State 22

   17 type: "dupl" •

    $default  reduce using rule 17 (type)


State 23

   18 type: "str" •

    $default  reduce using rule 18 (type)


State 24

   27 exodus-type: "anef" •

    $default  reduce using rule 27 (exodus-type)


State 25

   19 type: custom-type •

    $default  reduce using rule 19 (type)


State 26

   28 exodus-type: type •

    $default  reduce using rule 28 (exodus-type)


State 27

   25 func: "oper" ":" exodus-type • func-id "(" param-list ")" "{" func-body "}"
   26 func-id: • _ID

    _ID  shift, and go to state 35

    func-id  go to state 36


State 28

   16 type: "int" •

    $default  reduce using rule 16 (type)


State 29

    7 classdef: "exemp" ":" custom-type "{" class-body • "}"

    "}"  shift, and go to state 37


State 30

    9 class-body: attr-decl-list •
   10           | attr-decl-list • func-list
   23 func-list: • func
   24          | • func-list func
   25 func: • "oper" ":" exodus-type func-id "(" param-list ")" "{" func-body "}"

    "oper"  shift, and go to state 38

    $default  reduce using rule 9 (class-body)

    func-list  go to state 39
    func       go to state 13


State 31

    8 custom-type: • _ID
   12 attr-decl-list: non-empty-attr-decl-list •
   14 non-empty-attr-decl-list: non-empty-attr-decl-list • attr-decl
   15 attr-decl: • type _ID ";"
   16 type: • "int"
   17     | • "dupl"
   18     | • "str"
   19     | • custom-type

    "int"   shift, and go to state 28
    "dupl"  shift, and go to state 22
    "str"   shift, and go to state 23
    _ID     shift, and go to state 15

    $default  reduce using rule 12 (attr-decl-list)

    custom-type  go to state 25
    attr-decl    go to state 40
    type         go to state 33


State 32

   13 non-empty-attr-decl-list: attr-decl •

    $default  reduce using rule 13 (non-empty-attr-decl-list)


State 33

   15 attr-decl: type • _ID ";"

    _ID  shift, and go to state 41


State 34

   22 initus-func: "oper" ":" "int" "initus" • "(" ")" "{" func-body "}"

    "("  shift, and go to state 42


State 35

   26 func-id: _ID •

    $default  reduce using rule 26 (func-id)


State 36

   25 func: "oper" ":" exodus-type func-id • "(" param-list ")" "{" func-body "}"

    "("  shift, and go to state 43


State 37

    7 classdef: "exemp" ":" custom-type "{" class-body "}" •

    $default  reduce using rule 7 (classdef)


State 38

   25 func: "oper" • ":" exodus-type func-id "(" param-list ")" "{" func-body "}"

    ":"  shift, and go to state 44


State 39

   10 class-body: attr-decl-list func-list •
   24 func-list: func-list • func
   25 func: • "oper" ":" exodus-type func-id "(" param-list ")" "{" func-body "}"

    "oper"  shift, and go to state 38

    $default  reduce using rule 10 (class-body)

    func  go to state 19


State 40

   14 non-empty-attr-decl-list: non-empty-attr-decl-list attr-decl •

    $default  reduce using rule 14 (non-empty-attr-decl-list)


State 41

   15 attr-decl: type _ID • ";"

    ";"  shift, and go to state 45


State 42

   22 initus-func: "oper" ":" "int" "initus" "(" • ")" "{" func-body "}"

    ")"  shift, and go to state 46


State 43

    8 custom-type: • _ID
   16 type: • "int"
   17     | • "dupl"
   18     | • "str"
   19     | • custom-type
   25 func: "oper" ":" exodus-type func-id "(" • param-list ")" "{" func-body "}"
   29 param-list: ε •
   30           | • non-empty-param-list
   31 non-empty-param-list: • param
   32                     | • non-empty-param-list "," param
   33 param: • type param-id

    "int"   shift, and go to state 28
    "dupl"  shift, and go to state 22
    "str"   shift, and go to state 23
    _ID     shift, and go to state 15

    $default  reduce using rule 29 (param-list)

    custom-type           go to state 25
    type                  go to state 47
    param-list            go to state 48
    non-empty-param-list  go to state 49
    param                 go to state 50


State 44

    8 custom-type: • _ID
   16 type: • "int"
   17     | • "dupl"
   18     | • "str"
   19     | • custom-type
   25 func: "oper" ":" • exodus-type func-id "(" param-list ")" "{" func-body "}"
   27 exodus-type: • "anef"
   28            | • type

    "int"   shift, and go to state 28
    "dupl"  shift, and go to state 22
    "str"   shift, and go to state 23
    "anef"  shift, and go to state 24
    _ID     shift, and go to state 15

    custom-type  go to state 25
    type         go to state 26
    exodus-type  go to state 27


State 45

   15 attr-decl: type _ID ";" •

    $default  reduce using rule 15 (attr-decl)


State 46

   22 initus-func: "oper" ":" "int" "initus" "(" ")" • "{" func-body "}"

    "{"  shift, and go to state 51


State 47

   33 param: type • param-id
   34 param-id: • _ID

    _ID  shift, and go to state 52

    param-id  go to state 53


State 48

   25 func: "oper" ":" exodus-type func-id "(" param-list • ")" "{" func-body "}"

    ")"  shift, and go to state 54


State 49

   30 param-list: non-empty-param-list •
   32 non-empty-param-list: non-empty-param-list • "," param

    ","  shift, and go to state 55

    $default  reduce using rule 30 (param-list)


State 50

   31 non-empty-param-list: param •

    $default  reduce using rule 31 (non-empty-param-list)


State 51

   22 initus-func: "oper" ":" "int" "initus" "(" ")" "{" • func-body "}"
   35 func-body: • statement exodus-stm
   37 statement: • empty-stm
   38          | • non-empty-stm
   39 non-empty-stm: • compound-stm
   40              | • assign-stm
   41              | • exp
   42 empty-stm: ε •
   43 compound-stm: • assign-or-exp-stm ";" assign-or-exp-stm ";"
   44             | • compound-stm assign-or-exp-stm ";"
   45 assign-or-exp-stm: • assign-stm
   46                  | • exp
   47 assign-stm: • ids "=" exp
   48 ids: • _ID
   49 exp: • "(" exp ")"
   50    | • binop-exp
   51    | • func-call
   52    | • literal
   53    | • variable
   54 binop-exp: • exp binop exp
   59 func-call: • ids "(" arg-list ")"
   66 literal: • _INT
   67        | • _DUPL
   68        | • _STR
   69        | • "anef"
   70 variable: • _ID

    "anef"  shift, and go to state 56
    "("     shift, and go to state 57
    _ID     shift, and go to state 58
    _INT    shift, and go to state 59
    _DUPL   shift, and go to state 60
    _STR    shift, and go to state 61

    $default  reduce using rule 42 (empty-stm)

    func-body          go to state 62
    statement          go to state 63
    non-empty-stm      go to state 64
    empty-stm          go to state 65
    compound-stm       go to state 66
    assign-or-exp-stm  go to state 67
    assign-stm         go to state 68
    ids                go to state 69
    exp                go to state 70
    binop-exp          go to state 71
    func-call          go to state 72
    literal            go to state 73
    variable           go to state 74


State 52

   34 param-id: _ID •

    $default  reduce using rule 34 (param-id)


State 53

   33 param: type param-id •

    $default  reduce using rule 33 (param)


State 54

   25 func: "oper" ":" exodus-type func-id "(" param-list ")" • "{" func-body "}"

    "{"  shift, and go to state 75


State 55

    8 custom-type: • _ID
   16 type: • "int"
   17     | • "dupl"
   18     | • "str"
   19     | • custom-type
   32 non-empty-param-list: non-empty-param-list "," • param
   33 param: • type param-id

    "int"   shift, and go to state 28
    "dupl"  shift, and go to state 22
    "str"   shift, and go to state 23
    _ID     shift, and go to state 15

    custom-type  go to state 25
    type         go to state 47
    param        go to state 76


State 56

   69 literal: "anef" •

    $default  reduce using rule 69 (literal)


State 57

   48 ids: • _ID
   49 exp: • "(" exp ")"
   49    | "(" • exp ")"
   50    | • binop-exp
   51    | • func-call
   52    | • literal
   53    | • variable
   54 binop-exp: • exp binop exp
   59 func-call: • ids "(" arg-list ")"
   66 literal: • _INT
   67        | • _DUPL
   68        | • _STR
   69        | • "anef"
   70 variable: • _ID

    "anef"  shift, and go to state 56
    "("     shift, and go to state 57
    _ID     shift, and go to state 58
    _INT    shift, and go to state 59
    _DUPL   shift, and go to state 60
    _STR    shift, and go to state 61

    ids        go to state 77
    exp        go to state 78
    binop-exp  go to state 71
    func-call  go to state 72
    literal    go to state 73
    variable   go to state 74


State 58

   48 ids: _ID •
   70 variable: _ID •

    "("       reduce using rule 48 (ids)
    "="       reduce using rule 48 (ids)
    $default  reduce using rule 70 (variable)


State 59

   66 literal: _INT •

    $default  reduce using rule 66 (literal)


State 60

   67 literal: _DUPL •

    $default  reduce using rule 67 (literal)


State 61

   68 literal: _STR •

    $default  reduce using rule 68 (literal)


State 62

   22 initus-func: "oper" ":" "int" "initus" "(" ")" "{" func-body • "}"

    "}"  shift, and go to state 79


State 63

   35 func-body: statement • exodus-stm
   36 exodus-stm: • "exodus" exp ";"

    "exodus"  shift, and go to state 80

    exodus-stm  go to state 81


State 64

   38 statement: non-empty-stm •

    $default  reduce using rule 38 (statement)


State 65

   37 statement: empty-stm •

    $default  reduce using rule 37 (statement)


State 66

   39 non-empty-stm: compound-stm •
   44 compound-stm: compound-stm • assign-or-exp-stm ";"
   45 assign-or-exp-stm: • assign-stm
   46                  | • exp
   47 assign-stm: • ids "=" exp
   48 ids: • _ID
   49 exp: • "(" exp ")"
   50    | • binop-exp
   51    | • func-call
   52    | • literal
   53    | • variable
   54 binop-exp: • exp binop exp
   59 func-call: • ids "(" arg-list ")"
   66 literal: • _INT
   67        | • _DUPL
   68        | • _STR
   69        | • "anef"
   70 variable: • _ID

    "anef"  shift, and go to state 56
    "("     shift, and go to state 57
    _ID     shift, and go to state 58
    _INT    shift, and go to state 59
    _DUPL   shift, and go to state 60
    _STR    shift, and go to state 61

    $default  reduce using rule 39 (non-empty-stm)

    assign-or-exp-stm  go to state 82
    assign-stm         go to state 83
    ids                go to state 69
    exp                go to state 84
    binop-exp          go to state 71
    func-call          go to state 72
    literal            go to state 73
    variable           go to state 74


State 67

   43 compound-stm: assign-or-exp-stm • ";" assign-or-exp-stm ";"

    ";"  shift, and go to state 85


State 68

   40 non-empty-stm: assign-stm •
   45 assign-or-exp-stm: assign-stm •

    ";"       reduce using rule 45 (assign-or-exp-stm)
    $default  reduce using rule 40 (non-empty-stm)


State 69

   47 assign-stm: ids • "=" exp
   59 func-call: ids • "(" arg-list ")"

    "("  shift, and go to state 86
    "="  shift, and go to state 87


State 70

   41 non-empty-stm: exp •
   46 assign-or-exp-stm: exp •
   54 binop-exp: exp • binop exp
   55 binop: • _PLUS
   56      | • _MINUS
   57      | • _DIV
   58      | • _TIMES

    _PLUS   shift, and go to state 88
    _MINUS  shift, and go to state 89
    _DIV    shift, and go to state 90
    _TIMES  shift, and go to state 91

    ";"       reduce using rule 46 (assign-or-exp-stm)
    $default  reduce using rule 41 (non-empty-stm)

    binop  go to state 92


State 71

   50 exp: binop-exp •

    $default  reduce using rule 50 (exp)


State 72

   51 exp: func-call •

    $default  reduce using rule 51 (exp)


State 73

   52 exp: literal •

    $default  reduce using rule 52 (exp)


State 74

   53 exp: variable •

    $default  reduce using rule 53 (exp)


State 75

   25 func: "oper" ":" exodus-type func-id "(" param-list ")" "{" • func-body "}"
   35 func-body: • statement exodus-stm
   37 statement: • empty-stm
   38          | • non-empty-stm
   39 non-empty-stm: • compound-stm
   40              | • assign-stm
   41              | • exp
   42 empty-stm: ε •
   43 compound-stm: • assign-or-exp-stm ";" assign-or-exp-stm ";"
   44             | • compound-stm assign-or-exp-stm ";"
   45 assign-or-exp-stm: • assign-stm
   46                  | • exp
   47 assign-stm: • ids "=" exp
   48 ids: • _ID
   49 exp: • "(" exp ")"
   50    | • binop-exp
   51    | • func-call
   52    | • literal
   53    | • variable
   54 binop-exp: • exp binop exp
   59 func-call: • ids "(" arg-list ")"
   66 literal: • _INT
   67        | • _DUPL
   68        | • _STR
   69        | • "anef"
   70 variable: • _ID

    "anef"  shift, and go to state 56
    "("     shift, and go to state 57
    _ID     shift, and go to state 58
    _INT    shift, and go to state 59
    _DUPL   shift, and go to state 60
    _STR    shift, and go to state 61

    $default  reduce using rule 42 (empty-stm)

    func-body          go to state 93
    statement          go to state 63
    non-empty-stm      go to state 64
    empty-stm          go to state 65
    compound-stm       go to state 66
    assign-or-exp-stm  go to state 67
    assign-stm         go to state 68
    ids                go to state 69
    exp                go to state 70
    binop-exp          go to state 71
    func-call          go to state 72
    literal            go to state 73
    variable           go to state 74


State 76

   32 non-empty-param-list: non-empty-param-list "," param •

    $default  reduce using rule 32 (non-empty-param-list)


State 77

   59 func-call: ids • "(" arg-list ")"

    "("  shift, and go to state 86


State 78

   49 exp: "(" exp • ")"
   54 binop-exp: exp • binop exp
   55 binop: • _PLUS
   56      | • _MINUS
   57      | • _DIV
   58      | • _TIMES

    _PLUS   shift, and go to state 88
    _MINUS  shift, and go to state 89
    _DIV    shift, and go to state 90
    _TIMES  shift, and go to state 91
    ")"     shift, and go to state 94

    binop  go to state 92


State 79

   22 initus-func: "oper" ":" "int" "initus" "(" ")" "{" func-body "}" •

    $default  reduce using rule 22 (initus-func)


State 80

   36 exodus-stm: "exodus" • exp ";"
   48 ids: • _ID
   49 exp: • "(" exp ")"
   50    | • binop-exp
   51    | • func-call
   52    | • literal
   53    | • variable
   54 binop-exp: • exp binop exp
   59 func-call: • ids "(" arg-list ")"
   66 literal: • _INT
   67        | • _DUPL
   68        | • _STR
   69        | • "anef"
   70 variable: • _ID

    "anef"  shift, and go to state 56
    "("     shift, and go to state 57
    _ID     shift, and go to state 58
    _INT    shift, and go to state 59
    _DUPL   shift, and go to state 60
    _STR    shift, and go to state 61

    ids        go to state 77
    exp        go to state 95
    binop-exp  go to state 71
    func-call  go to state 72
    literal    go to state 73
    variable   go to state 74


State 81

   35 func-body: statement exodus-stm •

    $default  reduce using rule 35 (func-body)


State 82

   44 compound-stm: compound-stm assign-or-exp-stm • ";"

    ";"  shift, and go to state 96


State 83

   45 assign-or-exp-stm: assign-stm •

    $default  reduce using rule 45 (assign-or-exp-stm)


State 84

   46 assign-or-exp-stm: exp •
   54 binop-exp: exp • binop exp
   55 binop: • _PLUS
   56      | • _MINUS
   57      | • _DIV
   58      | • _TIMES

    _PLUS   shift, and go to state 88
    _MINUS  shift, and go to state 89
    _DIV    shift, and go to state 90
    _TIMES  shift, and go to state 91

    $default  reduce using rule 46 (assign-or-exp-stm)

    binop  go to state 92


State 85

   43 compound-stm: assign-or-exp-stm ";" • assign-or-exp-stm ";"
   45 assign-or-exp-stm: • assign-stm
   46                  | • exp
   47 assign-stm: • ids "=" exp
   48 ids: • _ID
   49 exp: • "(" exp ")"
   50    | • binop-exp
   51    | • func-call
   52    | • literal
   53    | • variable
   54 binop-exp: • exp binop exp
   59 func-call: • ids "(" arg-list ")"
   66 literal: • _INT
   67        | • _DUPL
   68        | • _STR
   69        | • "anef"
   70 variable: • _ID

    "anef"  shift, and go to state 56
    "("     shift, and go to state 57
    _ID     shift, and go to state 58
    _INT    shift, and go to state 59
    _DUPL   shift, and go to state 60
    _STR    shift, and go to state 61

    assign-or-exp-stm  go to state 97
    assign-stm         go to state 83
    ids                go to state 69
    exp                go to state 84
    binop-exp          go to state 71
    func-call          go to state 72
    literal            go to state 73
    variable           go to state 74


State 86

   59 func-call: ids "(" • arg-list ")"
   60 arg-list: ε •
   61         | • non-empty-arg-list
   62 non-empty-arg-list: • arg
   63                   | • non-empty-arg-list "," arg
   64 arg: • arg-id "=" exp
   65 arg-id: • _ID

    _ID  shift, and go to state 98

    $default  reduce using rule 60 (arg-list)

    arg-list            go to state 99
    non-empty-arg-list  go to state 100
    arg                 go to state 101
    arg-id              go to state 102


State 87

   47 assign-stm: ids "=" • exp
   48 ids: • _ID
   49 exp: • "(" exp ")"
   50    | • binop-exp
   51    | • func-call
   52    | • literal
   53    | • variable
   54 binop-exp: • exp binop exp
   59 func-call: • ids "(" arg-list ")"
   66 literal: • _INT
   67        | • _DUPL
   68        | • _STR
   69        | • "anef"
   70 variable: • _ID

    "anef"  shift, and go to state 56
    "("     shift, and go to state 57
    _ID     shift, and go to state 58
    _INT    shift, and go to state 59
    _DUPL   shift, and go to state 60
    _STR    shift, and go to state 61

    ids        go to state 77
    exp        go to state 103
    binop-exp  go to state 71
    func-call  go to state 72
    literal    go to state 73
    variable   go to state 74


State 88

   55 binop: _PLUS •

    $default  reduce using rule 55 (binop)


State 89

   56 binop: _MINUS •

    $default  reduce using rule 56 (binop)


State 90

   57 binop: _DIV •

    $default  reduce using rule 57 (binop)


State 91

   58 binop: _TIMES •

    $default  reduce using rule 58 (binop)


State 92

   48 ids: • _ID
   49 exp: • "(" exp ")"
   50    | • binop-exp
   51    | • func-call
   52    | • literal
   53    | • variable
   54 binop-exp: • exp binop exp
   54          | exp binop • exp
   59 func-call: • ids "(" arg-list ")"
   66 literal: • _INT
   67        | • _DUPL
   68        | • _STR
   69        | • "anef"
   70 variable: • _ID

    "anef"  shift, and go to state 56
    "("     shift, and go to state 57
    _ID     shift, and go to state 58
    _INT    shift, and go to state 59
    _DUPL   shift, and go to state 60
    _STR    shift, and go to state 61

    ids        go to state 77
    exp        go to state 104
    binop-exp  go to state 71
    func-call  go to state 72
    literal    go to state 73
    variable   go to state 74


State 93

   25 func: "oper" ":" exodus-type func-id "(" param-list ")" "{" func-body • "}"

    "}"  shift, and go to state 105


State 94

   49 exp: "(" exp ")" •

    $default  reduce using rule 49 (exp)


State 95

   36 exodus-stm: "exodus" exp • ";"
   54 binop-exp: exp • binop exp
   55 binop: • _PLUS
   56      | • _MINUS
   57      | • _DIV
   58      | • _TIMES

    _PLUS   shift, and go to state 88
    _MINUS  shift, and go to state 89
    _DIV    shift, and go to state 90
    _TIMES  shift, and go to state 91
    ";"     shift, and go to state 106

    binop  go to state 92


State 96

   44 compound-stm: compound-stm assign-or-exp-stm ";" •

    $default  reduce using rule 44 (compound-stm)


State 97

   43 compound-stm: assign-or-exp-stm ";" assign-or-exp-stm • ";"

    ";"  shift, and go to state 107


State 98

   65 arg-id: _ID •

    $default  reduce using rule 65 (arg-id)


State 99

   59 func-call: ids "(" arg-list • ")"

    ")"  shift, and go to state 108


State 100

   61 arg-list: non-empty-arg-list •
   63 non-empty-arg-list: non-empty-arg-list • "," arg

    ","  shift, and go to state 109

    $default  reduce using rule 61 (arg-list)


State 101

   62 non-empty-arg-list: arg •

    $default  reduce using rule 62 (non-empty-arg-list)


State 102

   64 arg: arg-id • "=" exp

    "="  shift, and go to state 110


State 103

   47 assign-stm: ids "=" exp •
   54 binop-exp: exp • binop exp
   55 binop: • _PLUS
   56      | • _MINUS
   57      | • _DIV
   58      | • _TIMES

    _PLUS   shift, and go to state 88
    _MINUS  shift, and go to state 89
    _DIV    shift, and go to state 90
    _TIMES  shift, and go to state 91

    $default  reduce using rule 47 (assign-stm)

    binop  go to state 92


State 104

   54 binop-exp: exp • binop exp
   54          | exp binop exp •
   55 binop: • _PLUS
   56      | • _MINUS
   57      | • _DIV
   58      | • _TIMES

    $default  reduce using rule 54 (binop-exp)

    binop  go to state 92


State 105

   25 func: "oper" ":" exodus-type func-id "(" param-list ")" "{" func-body "}" •

    $default  reduce using rule 25 (func)


State 106

   36 exodus-stm: "exodus" exp ";" •

    $default  reduce using rule 36 (exodus-stm)


State 107

   43 compound-stm: assign-or-exp-stm ";" assign-or-exp-stm ";" •

    $default  reduce using rule 43 (compound-stm)


State 108

   59 func-call: ids "(" arg-list ")" •

    $default  reduce using rule 59 (func-call)


State 109

   63 non-empty-arg-list: non-empty-arg-list "," • arg
   64 arg: • arg-id "=" exp
   65 arg-id: • _ID

    _ID  shift, and go to state 98

    arg     go to state 111
    arg-id  go to state 102


State 110

   48 ids: • _ID
   49 exp: • "(" exp ")"
   50    | • binop-exp
   51    | • func-call
   52    | • literal
   53    | • variable
   54 binop-exp: • exp binop exp
   59 func-call: • ids "(" arg-list ")"
   64 arg: arg-id "=" • exp
   66 literal: • _INT
   67        | • _DUPL
   68        | • _STR
   69        | • "anef"
   70 variable: • _ID

    "anef"  shift, and go to state 56
    "("     shift, and go to state 57
    _ID     shift, and go to state 58
    _INT    shift, and go to state 59
    _DUPL   shift, and go to state 60
    _STR    shift, and go to state 61

    ids        go to state 77
    exp        go to state 112
    binop-exp  go to state 71
    func-call  go to state 72
    literal    go to state 73
    variable   go to state 74


State 111

   63 non-empty-arg-list: non-empty-arg-list "," arg •

    $default  reduce using rule 63 (non-empty-arg-list)


State 112

   54 binop-exp: exp • binop exp
   55 binop: • _PLUS
   56      | • _MINUS
   57      | • _DIV
   58      | • _TIMES
   64 arg: arg-id "=" exp •

    _PLUS   shift, and go to state 88
    _MINUS  shift, and go to state 89
    _DIV    shift, and go to state 90
    _TIMES  shift, and go to state 91

    $default  reduce using rule 64 (arg)

    binop  go to state 92
