#include "asts.h"
#include "parser.h"

int getTokenPrec(TOKEN_TYPE t){
    switch (t){
    case TOK_OP_ASSIGN:
        return 5;
    case TOK_OP_SUB:
    case TOK_OP_ADD:
        return 10;
    case TOK_OP_DIV:
    case TOK_OP_MUL:
        return 20;
    default:
        return -1;
    }

}

Parser::Parser(string file): t(file) {

}

Parser::~Parser() {

}

ASTNode* Parser::parseProgram() {
    t.consume();
    if (t.match(TOK_SEMICOLON))
    return parseProgram();
    if (t.match(TOK_ID) && t.peekNext().type == TOK_COLON)
        return parseLabel();
    return parseStatement();
}

ASTNode* Parser::parseStatement() {
    switch (t.peek().type){
    case TOK_IF:
        return parseIfGoto();
        break;
    case TOK_GOTO:
        return parseGoto();
        break;
    case TOK_VAR:
        return parseDeclare();
        break;
    case TOK_EOF:
        return nullptr;
    default:
        return parseExpression();
    }
}

ASTNode* Parser::parseLabel() {
    t.expect(TOK_ID, "Excepet id Token");
    string id = t.peek().id;
    t.consume();
    t.expect(TOK_COLON, "Excepet colon token");
    cerr << "Good label found" << endl;
    return new LabelNode(id);
}

ASTNode* Parser::parseIfGoto() {
    t.expect(TOK_IF, "Except if token");
    t.consume();
    t.expect(TOK_OP_LEFTPAR, "Except left par token");
    t.consume();
    ASTNode* exprNode = parseExpression();
    t.expect(TOK_OP_RIGHTPAR, "Except right par token");
    t.consume();
    ASTNode* gotoStatement = parseGoto(exprNode);
    cerr << "Good if statement found" << endl;
    return gotoStatement;
}

ASTNode* Parser::parseGoto(ASTNode* cond) {
    assert(t.expect(TOK_GOTO, "Except goto token"));
    t.consume();
    t.expect(TOK_ID, "Except id token");
    string targetLabel = t.peek().id;
    t.consume();
    t.expect(TOK_SEMICOLON, "Except semicolon");
    cerr << "Good goto statement found" << endl;
    return new GotoNode(targetLabel, cond);
}

ASTNode* Parser::parseDeclare() {
    assert(t.expect(TOK_VAR, "Except var token"));
    t.consume();
    assert(t.expect(TOK_ID, "Except id token"));
    string variableId = t.peek().id;
    t.consume();
    ASTNode* initValue = nullptr;
    if (t.match(TOK_OP_ASSIGN)){
        t.consume();
        cerr << "Good varible declare with init val found" << endl;
        initValue = parseExpression();
    }
    
    cerr << "Good varible declare found" << endl;
    return new DeclareNode(variableId, initValue);
}

ASTNode* Parser::parseIntConstant() {
    assert(t.expect(TOK_NUMBER, "Except number token"));
    int value = t.peek().val;
    t.consume();
    cerr << "Good int constant found" << endl;
    return new IntConstantNode(value);
}

ASTNode* Parser::parseExpression() {
    return parseBinaryRight(parseFactor(), 0);
}

ASTNode* Parser::parseBinaryRight(ASTNode* lhs, int minPrec) {
    while (true) {
        int curPrec = getTokenPrec(t.peek().type);
        string op = t.peek().id;
        if (curPrec < minPrec)
            return lhs;
        t.consume();
        ASTNode* rhs = parseFactor();

        if (curPrec < getTokenPrec(t.peek().type)){
            rhs = parseBinaryRight(rhs, curPrec + 1);
            if (!rhs){
                return nullptr;
            }
        }
        if (!rhs)
            return lhs;
        lhs = new BinaryExpressionNode(op, lhs, rhs);
        cerr << "meet op " << endl;
    }
    return nullptr;
}

ASTNode* Parser::parseFactor() {
    ASTNode* expr = nullptr;
    switch (t.peek().type){
        case TOK_ID:
            return parseIdentifierExpression();
            break;
        case TOK_NUMBER:
            return parseIntConstant();
            break;
        case TOK_OP_LEFTPAR:
            t.consume();
            expr = parseExpression();
            t.expect(TOK_OP_RIGHTPAR, "Execept right par");
            t.consume();
            return expr;
        default:
            return nullptr;
    }
}

ASTNode* Parser::parseIdentifierExpression() {
    string id = t.peek().id;
    t.consume();
    if (!t.match(TOK_OP_LEFTPAR)){
        cerr << "Good id expression found" << endl;
        return new IdExpressionNode(id);
    }

    t.consume();
    vector<ASTNode*> args;
    while (!t.match(TOK_OP_RIGHTPAR)) {
        ASTNode* arg = parseExpression();
        assert(arg != nullptr);
        args.push_back(arg);
        if (!t.match(TOK_OP_RIGHTPAR)) {
            t.expect(TOK_COMMA, "Except comma token");
            t.consume();
        }
    }
    t.consume();
    cerr << "Good function found, call with " << args.size() << " args" << endl;
    return new FuntionCallExpressionNode(id, args);
}

ASTNode* Parser::parseArgList() {

}