#ifndef __Tokenizer_Header__
#define __Tokenizer_Header__

#include <iostream>
#include <fstream>

using namespace std;

enum TOKEN_TYPE {
    TOK_OP_ADD, TOK_OP_SUB, TOK_OP_MUL, TOK_OP_DIV,
    TOK_OP_ASSIGN,
    TOK_OP_LEFTPAR, TOK_OP_RIGHTPAR,
    TOK_COLON, TOK_SEMICOLON,
    TOK_COMMA,
    TOK_IF, TOK_GOTO, TOK_VAR,
    TOK_NUMBER, TOK_ID,
    TOK_EOF
};

struct Token {
    TOKEN_TYPE type;
    int val;
    string id;
    friend std::ostream& operator<<(std::ostream&, Token);
};

class Tokenizer {
    ifstream inputCharStream;
    string fileName;
    Token currentToken;
    Token nextToken;
    char currentChar;
    int currentLine;
    int currentColumn;
public:
    Tokenizer(string) ;
    ~Tokenizer();
    Token consume();
    Token peek() const;
    Token peekNext() const;
    bool match(TOKEN_TYPE);
    bool expect(TOKEN_TYPE, string);
};

#endif