%{

%}

%union {
    int ival;
    double dval;
    char *sval;
}

%token <sval> OPER "oper"
%token <sval> EXEMP "exemp"
%token <sval> INT_TYPE "int"
%token <sval> DUPL_TYPE "dupl"
%token <sval> STR_TYPE "str"
%token <sval> ANEF "anef"
%token <sval> EGO "ego"
%token <sval> INITUS "initus"
%token <sval> EXODUS "exodus"
%left <sval> PLUS "+"
%left <sval> MINUS "-"
%left <sval> DIV "/"
%left <sval> TIMES "*"
%token <sval> LPAREN "("
%token <sval> RPAREN ")"
%token <sval> LBRACK "{"
%token <sval> RBRACK "}"
%token <sval> COLON ":"
%token <sval> SEMICOLON ";"
%token <sval> DOT "."
%token <sval> COMMA ","
%token <sval> EQUAL "="
%token <sval> ID
%token <ival> INT
%token <dval> DUPL
%token <sval> STR

%%

program:
    classdefs funcs

classdefs:
    classdef-list

classdef-list:
    %empty
    | non-empty-classdef-list

non-empty-classdef-list:
    classdef
    | non-empty-classdef-list classdef

classdef:
    EXEMP COLON custom-type LBRACK class-body RBRACK

custom-type:
    ID

class-body:
    attr-decl-list func-list

attr-decl-list:
    %empty
    | non-empty-attr-decl-list

non-empty-attr-decl-list:
    attr-decl
    | non-empty-attr-decl-list attr-decl

attr-decl:
    type ID SEMICOLON

type:
    INT_TYPE
    | DUPL_TYPE
    | STR_TYPE
    | custom-type

funcs:
    func-list initus-func

initus-func:
    OPER COLON INT_TYPE INITUS LPAREN RPAREN LBRACK func-body RBRACK

func-list:
    %empty
    | func
    | func-list func

func:
    OPER COLON exodus-type ID LPAREN param-list RPAREN LBRACK func-body RBRACK

exodus-type:
    ANEF
    | type

param-list:
    %empty
    | non-empty-param-list

non-empty-param-list:
    param
    | non-empty-param-list COMMA param

param:
    type ID

func-body:
    statement EXODUS exp

statement:
    empty-stm
    | non-empty-stm
    
non-empty-stm:
    compound-stm
    | assign-stm
    | exp

empty-stm:
    %empty

compound-stm:
    non-empty-stm SEMICOLON non-empty-stm SEMICOLON

assign-stm:
    ids EQUAL exp

ids:
    ID
    | ids DOT ID

exp:
    LPAREN exp RPAREN
    | binop-exp
    | func-call
    | literal

binop-exp:
    exp binop exp

func-call:
    ids LPAREN arg-list RPAREN

arg-list:
    %empty
    | non-empty-arg-list

non-empty-arg-list:
    arg
    | non-empty-arg-list COMMA arg

arg:
    ID EQUAL exp

binop:
    PLUS
    | MINUS
    | DIV
    | TIMES

literal:
    INT
    | DUPL
    | STR

%%
