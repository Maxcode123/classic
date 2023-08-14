%code top {
    #include <string>
    #include <string.h>
    #include "../lexical/tokens.h"

    using namespace std;
    
    void yyerror(std::string);
    int yylex(void);
    extern Token current_token;
}

%code requires {
    #include "ast/builder.h"
    #include "../utils/error_handler.h"
    #include "ast/errors.h"

    extern ASTBuilder builder;
    extern ErrorHandler<ASTBuildError> handler;

    void update_ast();
}

%union {
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
}

%define parse.error detailed


%type <prog> program
%type <classdef_list> classdefs
%type <classdef_list> classdef-list
%type <clsdef> classdef
%type <func_list> funcs
%type <func_list> func-list
%type <function> func
%type <function> initus-func
%type <param_list> param-list non-empty-param-list
%type <param_t> param
%type <func_body> func-body
%type <stm_t> statement
%type <stm_t> empty-stm
%type <stm_t> non-empty-stm
%type <cmp_stm_t> compound-stm
%type <asgn_stm_t> assign-stm
%type <exodus_stm> exodus-stm
%type <exp_t> exp
%type <ltr_exp> literal
%type <binop_exp> binop-exp
%type <funcall_exp> func-call
%type <var_exp> variable 
%type <arg_list> non-empty-arg-list
%type <arg_t> arg
%type <arg_list> arg-list
%type <sval> ids
%type <sval> type
%type <sval> custom-type
%type <sval> func-id
%type <sval> param-id
%type <sval> arg-id
%type <sval> exodus-type

%token <sval> _OPER "oper"
%token <sval> _EXEMP "exemp"
%token <sval> _INT_TYPE "int"
%token <sval> _DUPL_TYPE "dupl"
%token <sval> _STR_TYPE "str"
%token <sval> _ANEF "anef"
%token <sval> _EGO "ego"
%token <sval> _INITUS "initus"
%token <sval> _EXODUS "exodus"
%left <sval> _PLUS "+"
%left <sval> _MINUS "-"
%left <sval> _DIV "/"
%left <sval> _TIMES "*"
%token <sval> _LPAREN "("
%token <sval> _RPAREN ")"
%token <sval> _LBRACK "{"
%token <sval> _RBRACK "}"
%token <sval> _COLON ":"
%token <sval> _SEMICOLON ";"
%token <sval> _DOT "."
%token <sval> _COMMA ","
%token <sval> _EQUAL "="
%token <sval> _ID
%token <ival> _INT
%token <dval> _DUPL
%token <sval> _STR

%left BINOP

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
    _EXEMP _COLON custom-type _LBRACK class-body _RBRACK

custom-type:
    _ID { builder.store(current_token.semantval()); update_ast(); }

class-body:
    attr-decl-list
    | attr-decl-list func-list

attr-decl-list:
    %empty
    | non-empty-attr-decl-list

non-empty-attr-decl-list:
    attr-decl
    | non-empty-attr-decl-list attr-decl

attr-decl:
    type _ID _SEMICOLON

type:
    _INT_TYPE { builder.store("int"); update_ast(); }
    | _DUPL_TYPE { builder.store("dupl"); update_ast(); }
    | _STR_TYPE { builder.store("str"); update_ast(); }
    | custom-type

funcs:
    initus-func { builder.build_last_function_list(); update_ast(); }
    | func-list initus-func { builder.build_pair_function_list(); update_ast(); }

initus-func:
    _OPER _COLON _INT_TYPE _INITUS _LPAREN _RPAREN _LBRACK func-body _RBRACK { builder.build_initus_function(); update_ast(); }

func-list:
    func { builder.build_last_function_list(); update_ast(); }
    | func-list func { builder.build_pair_function_list(); update_ast(); }

func:
    _OPER _COLON exodus-type func-id _LPAREN param-list _RPAREN _LBRACK func-body _RBRACK { builder.build_function(); update_ast(); }

func-id:
    _ID { builder.store(current_token.semantval()); update_ast(); }

exodus-type:
    _ANEF { builder.store("anef"); update_ast(); }
    | type

param-list:
    %empty { builder.build_empty_param_list(); update_ast(); }
    | non-empty-param-list

non-empty-param-list:
    param { builder.build_last_param_list(); update_ast(); }
    | non-empty-param-list _COMMA param { builder.build_pair_param_list(); update_ast(); }

param:
    type param-id { builder.build_param(); update_ast(); }

param-id:
    _ID { builder.store(current_token.semantval()); update_ast(); }

func-body:
    statement exodus-stm { builder.build_function_body(); update_ast(); }

exodus-stm:
    _EXODUS exp _SEMICOLON { builder.build_exodus_statement(); update_ast(); }

statement:
    empty-stm
    | non-empty-stm
    
non-empty-stm:
    compound-stm
    | assign-stm
    | exp { builder.build_expression_statement(); update_ast(); }

empty-stm:
    %empty { builder.build_empty_statement(); update_ast(); }

compound-stm:
    assign-or-exp-stm _SEMICOLON assign-or-exp-stm _SEMICOLON { handler.handle<CompoundStatement, ASTBuilder>(&builder, &ASTBuilder::build_compound_statement); update_ast(); }
    | compound-stm assign-or-exp-stm _SEMICOLON { handler.handle<CompoundStatement, ASTBuilder>(&builder, &ASTBuilder::build_compound_statement); update_ast(); }

assign-or-exp-stm:
    assign-stm
    | exp { builder.build_expression_statement(); update_ast(); }

assign-stm:
    ids _EQUAL exp { builder.build_assign_statement(); update_ast(); }

ids:
    _ID { builder.store(current_token.semantval()); update_ast(); }

exp:
    _LPAREN exp _RPAREN { builder.build_parentheses_expression(); update_ast(); }
    | binop-exp
    | func-call
    | literal
    | variable

binop-exp:
    exp binop exp %prec BINOP { builder.build_binary_operation_expression(); update_ast(); }

binop:
    _PLUS { builder.store(BINARY_PLUS); update_ast(); }
    | _MINUS { builder.store(BINARY_MINUS); update_ast(); }
    | _DIV { builder.store(BINARY_DIV); update_ast(); }
    | _TIMES { builder.store(BINARY_TIMES); update_ast(); }

func-call:
    ids _LPAREN arg-list _RPAREN { builder.build_function_call_expression(); update_ast(); }

arg-list:
    %empty { builder.build_empty_argument_list(); update_ast(); }
    | non-empty-arg-list

non-empty-arg-list:
    arg { builder.build_last_argument_list(); update_ast(); }
    | non-empty-arg-list _COMMA arg { builder.build_pair_argument_list(); update_ast(); }

arg:
    arg-id _EQUAL exp { builder.build_argument(); update_ast(); }

arg-id:
    _ID { builder.store(current_token.semantval()); update_ast(); }

literal:
    _INT { builder.build_literal_expression(std::stoi(current_token.semantval())); update_ast(); }
    | _DUPL { builder.build_literal_expression(std::stod(current_token.semantval())); update_ast(); }
    | _STR { builder.build_literal_expression(current_token.semantval()); update_ast(); }
    | _ANEF { builder.build_literal_expression(BUILTIN_ANEF_TYPE); update_ast(); }

variable:
    _ID { builder.build_variable_expression(current_token.semantval()); update_ast(); }

%%
