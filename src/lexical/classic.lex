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

<INITIAL>nota {adjust(); current_token = Token(Lexeme::NOTA); return 1;}
<INITIAL>adesm {adjust(); current_token = Token(Lexeme::ADESM); return 1;}
<INITIAL>desm {adjust(); current_token = Token(Lexeme::DESM); return 1;}
<INITIAL>int {adjust(); current_token = Token(Lexeme::INT); return 1;}
<INITIAL>dupl {adjust(); current_token = Token(Lexeme::DUPL); return 1;}
<INITIAL>series {adjust(); current_token = Token(Lexeme::SERIES); return 1;}
<INITIAL>exemp {adjust(); current_token = Token(Lexeme::EXEMP); return 1;}
<INITIAL>oper {adjust(); current_token = Token(Lexeme::OPER); return 1;}
<INITIAL>anef {adjust(); current_token = Token(Lexeme::ANEF); return 1;}
<INITIAL>return {adjust(); current_token = Token(Lexeme::RETURN); return 1;}

<INITIAL>{id} {adjust(); current_token = Token(Lexeme::ID, yytext); return 1;}
<INITIAL>{int} {adjust(); current_token = Token(Lexeme::NUM_I, yytext); return 1;}
<INITIAL>{double} {adjust(); current_token = Token(Lexeme::NUM_D, yytext); return 1;}

<INITIAL>"+" {adjust(); current_token = Token(Lexeme::PLUS); return 1;}
<INITIAL>"-" {adjust(); current_token = Token(Lexeme::MINUS); return 1;}
<INITIAL>"*" {adjust(); current_token = Token(Lexeme::TIMES); return 1;}
<INITIAL>"/" {adjust(); current_token = Token(Lexeme::DIV); return 1;}
<INITIAL>"\\" {adjust(); current_token = Token(Lexeme::BACKSLASH); return 1;}
<INITIAL>"(" {adjust(); current_token = Token(Lexeme::LPAREN); return 1;}
<INITIAL>")" {adjust(); current_token = Token(Lexeme::RPAREN); return 1;}
<INITIAL>"{" {adjust(); current_token = Token(Lexeme::LBRACK); return 1;}
<INITIAL>"}" {adjust(); current_token = Token(Lexeme::RBRACK); return 1;}
<INITIAL>"." {adjust(); current_token = Token(Lexeme::DOT); return 1;}
<INITIAL>"\"" {adjust(); current_token = Token(Lexeme::DQUOTE); return 1;}
<INITIAL>"=" {adjust(); current_token = Token(Lexeme::EQUAL); return 1;}
<INITIAL>";" {adjust(); current_token = Token(Lexeme::SEMICOLON); return 1;}
<INITIAL>":" {adjust(); current_token = Token(Lexeme::COLON); return 1;}
<INITIAL>"," {adjust(); current_token = Token(Lexeme::COMMA); return 1;}

<INITIAL>" " {adjust(); continue;}
<INITIAL>\n	 {adjust(); continue;}
<INITIAL>\t {adjust(); continue;}
<INITIAL>.	 {adjust(); fprintf(stderr, "illegal token");}

<INITIAL>"/*" {adjust(); BEGIN COMMENT;}
<COMMENT>"*/" {adjust(); BEGIN INITIAL;}
<COMMENT>. {adjust();}
