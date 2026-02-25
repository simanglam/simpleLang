#include "tokenizer.h"

bool isWhiteSpace(char c) {
    return (c == ' ') || (c == '\n') || (c == '\r') || (c == '\t');
}

bool isAlpha(char c){
    return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
}

bool isNumber(char c){
    return (c >= '0') && (c <= '9');
}

Tokenizer::Tokenizer(string _fileName): fileName(_fileName), inputCharStream(_fileName), currentChar(' ') {
    inputCharStream.unsetf(ios_base::skipws);
    consume();
}

Tokenizer::~Tokenizer() { }

Token Tokenizer::consume() {
    currentToken = nextToken;
    while (isWhiteSpace(currentChar) && inputCharStream.good()) {
        inputCharStream >> currentChar;
    }

    if (!inputCharStream.good()){
        nextToken.type = TOK_EOF;
        nextToken.id = "EOF";
        return currentToken;
    }
    
    switch (currentChar) {
        case '+':
            nextToken.type = TOK_OP_ADD;
            nextToken.id = currentChar;
            inputCharStream >> currentChar;
            break;
        case '-':
            nextToken.type = TOK_OP_SUB;
            nextToken.id = currentChar;
            inputCharStream >> currentChar;
            break;
        case '*':
            nextToken.type = TOK_OP_MUL;
            nextToken.id = currentChar;
            inputCharStream >> currentChar;
            break;
        case '/':
            nextToken.type = TOK_OP_DIV;
            nextToken.id = currentChar;
            inputCharStream >> currentChar;
            break;
        case '=':
            nextToken.type = TOK_OP_ASSIGN;
            nextToken.id = currentChar;
            inputCharStream >> currentChar;
            break;
        case '(':
            nextToken.type = TOK_OP_LEFTPAR;
            nextToken.id = currentChar;
            inputCharStream >> currentChar;
            break;
        case ')':
            nextToken.type = TOK_OP_RIGHTPAR;
            nextToken.id = currentChar;
            inputCharStream >> currentChar;
            break;
        case ':':
            nextToken.type = TOK_COLON;
            nextToken.id = currentChar;
            inputCharStream >> currentChar;
            break;
        case ';':
            nextToken.type = TOK_SEMICOLON;
            nextToken.id = currentChar;
            inputCharStream >> currentChar;
            break;
        case ',':
            nextToken.type = TOK_COMMA;
            nextToken.id = currentChar;
            inputCharStream >> currentChar;
            break;
        default:
            string buffer("");
            if (isNumber(currentChar)){
                do {
                    buffer += currentChar;
                    inputCharStream >> currentChar;
                } while (isNumber(currentChar) && inputCharStream.good());
                nextToken.type = TOK_NUMBER;
                nextToken.id = buffer;
                nextToken.val = atoi(buffer.c_str());
            }
            else if (isAlpha(currentChar)){
                do {
                    buffer += currentChar;
                    inputCharStream >> currentChar;
                } while (isAlpha(currentChar) && inputCharStream.good());
                if (buffer == "if")
                    nextToken.type = TOK_IF;
                else if (buffer == "goto")
                    nextToken.type = TOK_GOTO;
                else if (buffer == "var")
                    nextToken.type = TOK_VAR;
                else {
                    nextToken.type = TOK_ID;
                    nextToken.id = buffer;
                }
            }
            break;
    }

    return currentToken;
}

Token Tokenizer::peek() const {
    return currentToken;
}

Token Tokenizer::peekNext() const {
    return nextToken;
}

bool Tokenizer::match(TOKEN_TYPE t) {
    return currentToken.type == t;
}

bool Tokenizer::expect(TOKEN_TYPE t, string s) {
    if (t != currentToken.type) {
        cout << s << endl;
        return false;
    }
    return true;
}