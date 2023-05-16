%{
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tokens.h"

extern int pos;
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

id [a-zA-Z][a-zA-Z0-9]*
int [-]?[0-9]+
double [-+]?[0-9]+\.?[0-9]*

%Start COMMENT
%%

<INITIAL>oper {adjust(); current_token = Token(Lexeme::OPER.name); return 1;}
<INITIAL>exemp {adjust(); current_token = Token(Lexeme::EXEMP.name); return 1;}
<INITIAL>int {adjust(); current_token = Token(Lexeme::INT_TYPE.name); return 1;}
<INITIAL>dupl {adjust(); current_token = Token(Lexeme::DUPL_TYPE.name); return 1;}
<INITIAL>str {adjust(); current_token = Token(Lexeme::STR_TYPE.name); return 1;}
<INITIAL>anef {adjust(); current_token = Token(Lexeme::ANEF.name); return 1;}
<INITIAL>ego {adjust(); current_token = Token(Lexeme::EGO.name); return 1;}
<INITIAL>initus {adjust(); current_token = Token(Lexeme::INITUS.name); return 1;}
<INITIAL>exodus {adjust(); current_token = Token(Lexeme::EXODUS.name); return 1;}

<INITIAL>{id} {adjust(); current_token = Token(Lexeme::ID.name, yytext); return 1;}
<INITIAL>{int} {adjust(); current_token = Token(Lexeme::INT.name, yytext); return 1;}
<INITIAL>{double} {adjust(); current_token = Token(Lexeme::DUPL.name, yytext); return 1;}

<INITIAL>"+" {adjust(); current_token = Token(Lexeme::PLUS.name); return 1;}
<INITIAL>"-" {adjust(); current_token = Token(Lexeme::MINUS.name); return 1;}
<INITIAL>"/" {adjust(); current_token = Token(Lexeme::DIV.name); return 1;}
<INITIAL>"*" {adjust(); current_token = Token(Lexeme::TIMES.name); return 1;}
<INITIAL>"(" {adjust(); current_token = Token(Lexeme::LPAREN.name); return 1;}
<INITIAL>")" {adjust(); current_token = Token(Lexeme::RPAREN.name); return 1;}
<INITIAL>"{" {adjust(); current_token = Token(Lexeme::LBRACK.name); return 1;}
<INITIAL>"}" {adjust(); current_token = Token(Lexeme::RBRACK.name); return 1;}
<INITIAL>":" {adjust(); current_token = Token(Lexeme::COLON.name); return 1;}
<INITIAL>";" {adjust(); current_token = Token(Lexeme::SEMICOLON.name); return 1;}
<INITIAL>"." {adjust(); current_token = Token(Lexeme::DOT.name); return 1;}
<INITIAL>"," {adjust(); current_token = Token(Lexeme::COMMA.name); return 1;}
<INITIAL>"=" {adjust(); current_token = Token(Lexeme::EQUAL.name); return 1;}

<INITIAL>" " {adjust(); continue;}
<INITIAL>\n	 {adjust(); continue;}
<INITIAL>\t {adjust(); continue;}
<INITIAL>.	 {adjust(); fprintf(stderr, "illegal token");}

<INITIAL>"/*" {adjust(); BEGIN COMMENT;}
<COMMENT>"*/" {adjust(); BEGIN INITIAL;}
<COMMENT>. {adjust();}
