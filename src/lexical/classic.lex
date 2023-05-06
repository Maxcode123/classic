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

<INITIAL>nota {adjust(); yylval = Token(Lexeme::NOTA); return 1;}
<INITIAL>adesm {adjust(); yylval = Token(Lexeme::ADESM); return 1;}
<INITIAL>desm {adjust(); yylval = Token(Lexeme::DESM); return 1;}
<INITIAL>int {adjust(); yylval = Token(Lexeme::INT); return 1;}
<INITIAL>dupl {adjust(); yylval = Token(Lexeme::DUPL); return 1;}
<INITIAL>series {adjust(); yylval = Token(Lexeme::SERIES); return 1;}
<INITIAL>exemp {adjust(); yylval = Token(Lexeme::EXEMP); return 1;}
<INITIAL>oper {adjust(); yylval = Token(Lexeme::OPER); return 1;}
<INITIAL>anef {adjust(); yylval = Token(Lexeme::ANEF); return 1;}
<INITIAL>return {adjust(); yylval = Token(Lexeme::RETURN); return 1;}

<INITIAL>{id} {adjust(); yylval = Token(Lexeme::ID, yytext); return 1;}
<INITIAL>{int} {adjust(); yylval = Token(Lexeme::NUM_I, yytext); return 1;}
<INITIAL>{double} {adjust(); yylval = Token(Lexeme::NUM_D, yytext); return 1;}

<INITIAL>"+" {adjust(); yylval = Token(Lexeme::PLUS); return 1;}
<INITIAL>"-" {adjust(); yylval = Token(Lexeme::MINUS); return 1;}
<INITIAL>"*" {adjust(); yylval = Token(Lexeme::TIMES); return 1;}
<INITIAL>"/" {adjust(); yylval = Token(Lexeme::DIV); return 1;}
<INITIAL>"\\" {adjust(); yylval = Token(Lexeme::BACKSLASH); return 1;}
<INITIAL>"(" {adjust(); yylval = Token(Lexeme::LPAREN); return 1;}
<INITIAL>")" {adjust(); yylval = Token(Lexeme::RPAREN); return 1;}
<INITIAL>"{" {adjust(); yylval = Token(Lexeme::LBRACK); return 1;}
<INITIAL>"}" {adjust(); yylval = Token(Lexeme::RBRACK); return 1;}
<INITIAL>"." {adjust(); yylval = Token(Lexeme::DOT); return 1;}
<INITIAL>"\"" {adjust(); yylval = Token(Lexeme::DQUOTE); return 1;}
<INITIAL>"=" {adjust(); yylval = Token(Lexeme::EQUAL); return 1;}
<INITIAL>";" {adjust(); yylval = Token(Lexeme::SEMICOLON); return 1;}
<INITIAL>":" {adjust(); yylval = Token(Lexeme::COLON); return 1;}
<INITIAL>"," {adjust(); yylval = Token(Lexeme::COMMA); return 1;}

<INITIAL>" " {adjust(); continue;}
<INITIAL>\n	 {adjust(); continue;}
<INITIAL>\t {adjust(); continue;}
<INITIAL>.	 {adjust(); fprintf(stderr, "illegal token");}

<INITIAL>"/*" {adjust(); BEGIN COMMENT;}
<COMMENT>"*/" {adjust(); BEGIN INITIAL;}
<COMMENT>. {adjust();}
