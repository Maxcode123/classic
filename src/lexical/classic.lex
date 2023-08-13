%{
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tokens.h"

Token current_token;
int pos;
int _pos=1;

extern "C" int yywrap(void)
{
 _pos=1;
 return 1;
}

void adjust(void)
{
 pos=_pos;
 _pos+=yyleng;
}

%}

id [a-zA-Z_][a-zA-Z0-9_]*
int [-]?[0-9]+
double [-+]?[0-9]+\.?[0-9]*

%Start COMMENT
%%

<INITIAL>oper {adjust(); current_token = Token(Lexeme::OPER.name); return Lexeme::OPER.value;}
<INITIAL>exemp {adjust(); current_token = Token(Lexeme::EXEMP.name); return Lexeme::EXEMP.value;}
<INITIAL>int {adjust(); current_token = Token(Lexeme::INT_TYPE.name); return Lexeme::INT_TYPE.value;}
<INITIAL>dupl {adjust(); current_token = Token(Lexeme::DUPL_TYPE.name); return Lexeme::DUPL_TYPE.value;}
<INITIAL>str {adjust(); current_token = Token(Lexeme::STR_TYPE.name); return Lexeme::STR_TYPE.value;}
<INITIAL>anef {adjust(); current_token = Token(Lexeme::ANEF.name); return Lexeme::ANEF.value;}
<INITIAL>ego {adjust(); current_token = Token(Lexeme::EGO.name); return Lexeme::EGO.value;}
<INITIAL>initus {adjust(); current_token = Token(Lexeme::INITUS.name); return Lexeme::INITUS.value;}
<INITIAL>exodus {adjust(); current_token = Token(Lexeme::EXODUS.name); return Lexeme::EXODUS.value;}

<INITIAL>{id} {adjust(); current_token = Token(Lexeme::ID.name, yytext); return Lexeme::ID.value;}
<INITIAL>{int} {adjust(); current_token = Token(Lexeme::INT.name, yytext); return Lexeme::INT.value;}
<INITIAL>{double} {adjust(); current_token = Token(Lexeme::DUPL.name, yytext); return Lexeme::DUPL.value;}

<INITIAL>"+" {adjust(); current_token = Token(Lexeme::PLUS.name); return Lexeme::PLUS.value;}
<INITIAL>"-" {adjust(); current_token = Token(Lexeme::MINUS.name); return Lexeme::MINUS.value;}
<INITIAL>"/" {adjust(); current_token = Token(Lexeme::DIV.name); return Lexeme::DIV.value;}
<INITIAL>"*" {adjust(); current_token = Token(Lexeme::TIMES.name); return Lexeme::TIMES.value;}
<INITIAL>"(" {adjust(); current_token = Token(Lexeme::LPAREN.name); return Lexeme::LPAREN.value;}
<INITIAL>")" {adjust(); current_token = Token(Lexeme::RPAREN.name); return Lexeme::RPAREN.value;}
<INITIAL>"{" {adjust(); current_token = Token(Lexeme::LBRACK.name); return Lexeme::LBRACK.value;}
<INITIAL>"}" {adjust(); current_token = Token(Lexeme::RBRACK.name); return Lexeme::RBRACK.value;}
<INITIAL>":" {adjust(); current_token = Token(Lexeme::COLON.name); return Lexeme::COLON.value;}
<INITIAL>";" {adjust(); current_token = Token(Lexeme::SEMICOLON.name); return Lexeme::SEMICOLON.value;}
<INITIAL>"." {adjust(); current_token = Token(Lexeme::DOT.name); return Lexeme::DOT.value;}
<INITIAL>"," {adjust(); current_token = Token(Lexeme::COMMA.name); return Lexeme::COMMA.value;}
<INITIAL>"=" {adjust(); current_token = Token(Lexeme::EQUAL.name); return Lexeme::EQUAL.value;}

<INITIAL>" " {adjust(); continue;}
<INITIAL>\n	 {adjust(); yylineno++; _pos=1; continue;}
<INITIAL>\t {adjust(); continue;}
<INITIAL>.	 {adjust(); fprintf(stderr, "illegal token");}
<INITIAL><<EOF>> {return 0;}

<INITIAL>"/*" {adjust(); BEGIN COMMENT;}
<COMMENT>"*/" {adjust(); BEGIN INITIAL;}
<COMMENT>. {adjust();}
