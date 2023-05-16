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

<INITIAL>oper {adjust(); current_token = Token(Lexeme::OPER); return 1;}
<INITIAL>exemp {adjust(); current_token = Token(Lexeme::EXEMP); return 1;}
<INITIAL>int {adjust(); current_token = Token(Lexeme::INT_TYPE); return 1;}
<INITIAL>dupl {adjust(); current_token = Token(Lexeme::DUPL_TYPE); return 1;}
<INITIAL>str {adjust(); current_token = Token(Lexeme::STR_TYPE); return 1;}
<INITIAL>anef {adjust(); current_token = Token(Lexeme::ANEF); return 1;}
<INITIAL>ego {adjust(); current_token = Token(Lexeme::EGO); return 1;}
<INITIAL>initus {adjust(); current_token = Token(Lexeme::INITUS); return 1;}
<INITIAL>exodus {adjust(); current_token = Token(Lexeme::EXODUS); return 1;}

<INITIAL>{id} {adjust(); current_token = Token(Lexeme::ID, yytext); return 1;}
<INITIAL>{int} {adjust(); current_token = Token(Lexeme::INT, yytext); return 1;}
<INITIAL>{double} {adjust(); current_token = Token(Lexeme::DUPL, yytext); return 1;}

<INITIAL>"+" {adjust(); current_token = Token(Lexeme::PLUS); return 1;}
<INITIAL>"-" {adjust(); current_token = Token(Lexeme::MINUS); return 1;}
<INITIAL>"/" {adjust(); current_token = Token(Lexeme::DIV); return 1;}
<INITIAL>"*" {adjust(); current_token = Token(Lexeme::TIMES); return 1;}
<INITIAL>"(" {adjust(); current_token = Token(Lexeme::LPAREN); return 1;}
<INITIAL>")" {adjust(); current_token = Token(Lexeme::RPAREN); return 1;}
<INITIAL>"{" {adjust(); current_token = Token(Lexeme::LBRACK); return 1;}
<INITIAL>"}" {adjust(); current_token = Token(Lexeme::RBRACK); return 1;}
<INITIAL>":" {adjust(); current_token = Token(Lexeme::COLON); return 1;}
<INITIAL>";" {adjust(); current_token = Token(Lexeme::SEMICOLON); return 1;}
<INITIAL>"." {adjust(); current_token = Token(Lexeme::DOT); return 1;}
<INITIAL>"," {adjust(); current_token = Token(Lexeme::COMMA); return 1;}
<INITIAL>"=" {adjust(); current_token = Token(Lexeme::EQUAL); return 1;}

<INITIAL>" " {adjust(); continue;}
<INITIAL>\n	 {adjust(); continue;}
<INITIAL>\t {adjust(); continue;}
<INITIAL>.	 {adjust(); fprintf(stderr, "illegal token");}

<INITIAL>"/*" {adjust(); BEGIN COMMENT;}
<COMMENT>"*/" {adjust(); BEGIN INITIAL;}
<COMMENT>. {adjust();}
