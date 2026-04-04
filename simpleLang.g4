grammar simpleLang;

options {language=Cpp;}

// Literal Tokens

LEFT_PAR: '(';
RIGHT_PAR: ')';
SEMICOLON: ';';
COLON: ':';

KEYWORD_IF: 'if';
KEYWORD_GOTO: 'goto';
KEYWORD_VAR: 'var';

OP_ADD: '+';
OP_SUB: '-';
OP_MUL: '*';
OP_DIV: '/';
OP_ASSIGN: '=';

// ID

NUM: (DIGIT)+;
ID: (LETTER)(LETTER | DIGIT | '_')*;

WS: (' ' | '\t' | '\r' | '\n')+ -> skip;

fragment LETTER: 'a'..'z' | 'A'..'Z';
fragment DIGIT: '0'..'9';


// Parsers

prog: (statement)+ EOF;

label:
    ID COLON;

statement:
    if_goto_statement ';'
    | goto_statement ';'
    | variable_declare_statement ';'
    | expr ';'
    | label
    ';'
    ;

if_goto_statement:
    KEYWORD_IF '(' expr ')' KEYWORD_GOTO ID;

goto_statement:
    KEYWORD_GOTO ID;

variable_declare_statement
    : KEYWORD_VAR ID (OP_ASSIGN expr)?
    ;

expr
    : assign_expr
    ;

assign_expr
    : ID OP_ASSIGN assign_expr
    | arithmetic_expr
    ;

arithmetic_expr
    : mult_expr ((OP_ADD | OP_SUB) mult_expr)*
    ;

mult_expr
    : atom ((OP_MUL | OP_DIV) atom)*
    ;

atom
    : NUM
    | ID
    | ID '(' expr* ')'
    | '(' expr ')'
    ;
