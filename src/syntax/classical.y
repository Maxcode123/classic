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
%token <sval> PLUS "+"
%token <sval> MINUS "-"
%token <sval> DIV "/"
%token <sval> TIMES "*"
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
    | classdef
    | classdef-list classdef

classdef:
    EXEMP COLON custom-type LBRACK class-body RBRACK

custom-type:
    ID

class-body:
    attr-decl-list func-list

attr-decl-list:
    %empty
    | attr-decl
    | attr-decl-list attr-decl

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
    | param-list-ne

param-list-ne:
    param
    | param-list-ne COMMA param

param:
    type ID

func-body:
    statement EXODUS exp

statement:
    empty-stm
    | compound-stm
    | assign-stm
    | exp

empty-stm:
    %empty

compound-stm:
    statement SEMICOLON statement SEMICOLON

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
    | arg-list-ne

arg-list-ne:
    arg
    | arg-list-ne COMMA arg

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
