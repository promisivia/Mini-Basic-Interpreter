#include "parser.h"

Parser::Parser()
{

}

int Parser::getPrecedence(QString op)
{
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;
}

Expression* Parser::scanExpression(Tokenizer* tokenizer)
{
    QString firstToken = tokenizer->getNextToken();
    if (firstToken == "-"){
        exp.push(new ConstantExp(0));
    }
    tokenizer->trackback();

    while(tokenizer->hasMoreTokens()){
        QString token = tokenizer->getNextToken();
        TokenType type = tokenizer->getTokenType(token);

        if(type == NUMBER)
            exp.push(new ConstantExp(token.toInt()));
        else if(type == WORD)
            exp.push(new IdentifierExp(token));
        else if(type == OPERATOR && token != "=" && token != ">" && token != "<"){
            if (token == "(")
                op.push(token);
            else if (token == ")"){
                QString oper = op.pop();
                while( oper!= "(" ){
                    Expression* right = exp.pop();
                    Expression* left = exp.pop();
                    exp.push(new CompoundExp(oper, left, right));
                    oper = op.pop();
                }
            }else{
                if(!op.isEmpty()){
                    int nowPre = getPrecedence(token);
                    int topPre = getPrecedence(op.top());
                    while(topPre >= nowPre && !op.isEmpty() && op.top()!="(" ){
                        Expression* right = exp.pop();
                        Expression* left = exp.pop();
                        exp.push(new CompoundExp(op.pop(), left, right));
                        if(op.isEmpty())break;
                        topPre = getPrecedence(op.top());
                    }
                }
                op.push(token);
            }
        }else {
            tokenizer->trackback();
            break;
        }
    }
    while(!op.isEmpty()){
        if(exp.isEmpty()) return nullptr;
        Expression* right = exp.pop();
        if(exp.isEmpty()) return nullptr;
        Expression* left = exp.pop();
        exp.push(new CompoundExp(op.pop(), left, right));
    }

    Expression* res = exp.pop();
    if(!exp.isEmpty()) {
        exp.clear();
        return nullptr;
    }
    return res;
}

Statement* Parser::parseStatement(Tokenizer* tokenizer, Basic* basic)
{
    QString statement = tokenizer->getNextToken();
    Statement *stm = nullptr;

    if(statement == "REM") {
        stm = new RemStmt(tokenizer,basic);
    } else if(statement == "LET") {
        stm = new LetStmt(tokenizer,basic,false);
    } else if(statement == "PRINT") {
        stm = new PrintStmt(tokenizer,basic);
    } else if(statement == "INPUT") {
        stm = new InputStmt(tokenizer,basic);
    } else if(statement == "GOTO") {
        stm = new GotoStmt(tokenizer,basic);
    } else if(statement == "IF") {
        stm = new IfStmt(tokenizer,basic);
    } else if(statement == "END") {
        stm = new EndStmt(tokenizer,basic);
    } else if(statement == "SUB") {
        stm = new SubStmt(tokenizer,basic);
    } else if(statement == "CALL") {
        stm = new CallStmt(tokenizer,basic);
    } else if(tokenizer->getTokenType(statement) == WORD) {
        tokenizer->trackback();
        stm = new LetStmt(tokenizer,basic,true);
    } else throw QString("Invald statement: " + statement);

    return stm;
}
