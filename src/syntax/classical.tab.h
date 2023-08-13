/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_SYNTAX_CLASSICAL_TAB_H_INCLUDED
# define YY_YY_SRC_SYNTAX_CLASSICAL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 13 "src/syntax/classical.y"

    #include "ast/builder.h"
    #include "../utils/error_handler.h"
    #include "ast/errors.h"

    extern ASTBuilder builder;
    extern ErrorHandler<ASTBuildError> handler;

    void update_ast();

#line 60 "src/syntax/classical.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    _OPER = 258,                   /* "oper"  */
    _EXEMP = 259,                  /* "exemp"  */
    _INT_TYPE = 260,               /* "int"  */
    _DUPL_TYPE = 261,              /* "dupl"  */
    _STR_TYPE = 262,               /* "str"  */
    _ANEF = 263,                   /* "anef"  */
    _EGO = 264,                    /* "ego"  */
    _INITUS = 265,                 /* "initus"  */
    _EXODUS = 266,                 /* "exodus"  */
    _PLUS = 267,                   /* _PLUS  */
    _MINUS = 269,                  /* _MINUS  */
    _DIV = 271,                    /* _DIV  */
    _TIMES = 273,                  /* _TIMES  */
    _LPAREN = 275,                 /* "("  */
    _RPAREN = 276,                 /* ")"  */
    _LBRACK = 277,                 /* "{"  */
    _RBRACK = 278,                 /* "}"  */
    _COLON = 279,                  /* ":"  */
    _SEMICOLON = 280,              /* ";"  */
    _DOT = 281,                    /* "."  */
    _COMMA = 282,                  /* ","  */
    _EQUAL = 283,                  /* "="  */
    _ID = 284,                     /* _ID  */
    _INT = 285,                    /* _INT  */
    _DUPL = 286,                   /* _DUPL  */
    _STR = 287,                    /* _STR  */
    BINOP = 288                    /* BINOP  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 24 "src/syntax/classical.y"

    int ival;
    double dval;
    char *sval;
    Program prog;
    ClassdefList classdef_list;
    Classdef clsdef;
    FunctionList func_list;
    Function function;
    ParamList param_list;
    Param param_t;
    FunctionBody func_body;
    Statement stm_t;
    CompoundStatement cmp_stm_t;
    AssignStatement asgn_stm_t;
    ExodusStatement exodus_stm;
    Expression exp_t;
    LiteralExpression ltr_exp;
    BinaryOperationExpression binop_exp;
    FunctionCallExpression funcall_exp;
    VariableExpression var_exp;
    ArgumentList arg_list;
    Argument arg_t;

#line 131 "src/syntax/classical.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SRC_SYNTAX_CLASSICAL_TAB_H_INCLUDED  */
