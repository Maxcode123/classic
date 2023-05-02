%{
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../utils/util.h"
#include "tokens.h"

extern int pos;
int _pos=1;

int yywrap(void)
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
num [-+]?[0-9]*\.?[0-9]

%Start COMMENT
%%

<INITIAL>nota {adjust(); return NOTA;}
<INITIAL>adesm {adjust(); return ADESM;}
<INITIAL>desm {adjust(); return DESM;}
<INITIAL>int {adjust(); return INT;}
<INITIAL>series {adjust(); return SERIES;}
<INITIAL>exemp {adjust(); return EXEMP;}
<INITIAL>oper {adjust(); return OPER;}
<INITIAL>anef {adjust(); return ANEF;}

<INITIAL>{id} {adjust(); yylval.sval = String(yytext); return ID;}
<INITIAL>{num} {adjust(); yylval.ival = atoi(yytext); return NUM;}

<INITIAL>"+" {adjust(); return PLUS;}
<INITIAL>"-" {adjust(); return MINUS;}
<INITIAL>"*" {adjust(); return TIMES;}
<INITIAL>"/" {adjust(); return DIV;}
<INITIAL>"\" {adjust(); return BACKSLASH;}
<INITIAL>"(" {adjust(); return LPAREN;}
<INITIAL>")" {adjust(); return RPAREN;}
<INITIAL>"{" {adjust(); return LBRACK;}
<INITIAL>"}" {adjust(); return RBRACK;}
<INITIAL>"." {adjust(); return DOT;}
<INITIAL>""" {adjust(); return DQUOTE;}
<INITIAL>"=" {adjust(); return EQUAL;}
<INITIAL>";" {adjust(); return SEMICOLON;}
<INITIAL>":" {adjust(); return COLON;}
<INITIAL>"," {adjust(); return COMMA;}

<INITIAL>" "	 {adjust(); continue;}
<INITIAL>\n	 {adjust(); continue;}
<INITIAL>\t {adjust(); continue;}
<INITIAL>.	 {adjust(); fprintf(stderr, "illegal token");}

<INITIAL>"/*" {adjust(); BEGIN COMMENT;}
<COMMENT>"*/" {adjust(); BEGIN INITIAL;}
<COMMENT>. {adjust();}