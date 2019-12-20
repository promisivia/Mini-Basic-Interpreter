/*
 * File: statement.h
 * -----------------
 * This file export class Statement and its concrete subclasses.
 */

#ifndef STATEMENT_H
#define STATEMENT_H

#include "tokenizer.h"
#include "expression.h"
#include <QCoreApplication>
#include <QTime>

class Basic;
class Program;

class Statement
{
public:
    Statement();
    virtual ~Statement();
protected:
    Tokenizer *tokenizer;
    Basic* basic;
    Program *program;
    Expression *exp;
public slots:
    virtual void execute(Evaluation* eval) = 0;
    int getLineNum();
};

class RemStmt : public Statement
{
public:
    RemStmt(Tokenizer* s, Basic* b);
    virtual void execute(Evaluation* eval);
};

class LetStmt : public Statement
{
private:
    QString var, op;
    int value;
    bool isDefined;
public:
    LetStmt(Tokenizer* s, Basic* b, bool flag);
    virtual void execute(Evaluation* eval);
};

class PrintStmt : public Statement
{
private:
    QString var;
public:
    PrintStmt(Tokenizer* s, Basic* b);
    virtual void execute(Evaluation* eval);
};

class InputStmt : public Statement
{
private:
    QString var;
public:
    InputStmt(Tokenizer* s, Basic* b);
    virtual void execute(Evaluation* eval);
    void waitSec(int s);
    int getValue();
};

class GotoStmt : public Statement
{
private:
    QString nextLineNum;
public:
    GotoStmt(Tokenizer* s, Basic* b);
    virtual void execute(Evaluation* eval);
};

class IfStmt : public Statement
{
private:
    Expression *expLeft;
    Expression *expRight;
    QString op;
    QString nextLineNum;
public:
    IfStmt(Tokenizer* s, Basic* b);
    virtual void execute(Evaluation* eval);
    bool checkCondition(Evaluation* eval);
};

class EndStmt : public Statement
{
private:
    QString funcName;
    int func_end;
public:
    EndStmt(Tokenizer* s, Basic* b);
    virtual void execute(Evaluation* eval);
};

class SubStmt : public Statement
{
private:
    QString funcName;
    int func_start;
public:
    SubStmt(Tokenizer* s, Basic* b);
    virtual void execute(Evaluation* eval);
};

class CallStmt : public Statement
{
private:
    QString funcName;
    int lineNum = -1;
public:
    CallStmt(Tokenizer* s, Basic* b);
    virtual void execute(Evaluation* eval);
};

#endif // STATEMENT_H
