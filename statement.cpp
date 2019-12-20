#include "statement.h"
#include "basic.h"

Statement::Statement()
{
}

Statement::~Statement()
{
}

int Statement::getLineNum()
{
    return tokenizer->getLineNum();
}

RemStmt::RemStmt(Tokenizer* s, Basic* b)
{
    tokenizer = s;
    basic = b;
    program = b->program;
    program->setParsed(this);
}

void RemStmt::execute(Evaluation* eval)
{
    eval->setNextLineNum(-1);
}

LetStmt::LetStmt(Tokenizer* s, Basic* b, bool flag)
{
    tokenizer = s;
    basic = b;
    program = b->program;
    isDefined = flag;

    var = tokenizer->getNextToken();

    op = tokenizer->getNextToken();
    if(op != "=")
        throw "Invalid operate: "+op;

    this->exp = b->parser->scanExpression(s);
    if (exp == nullptr)
        throw QString("Invalid expression.");

    program->setParsed(this);
}

void LetStmt::execute(Evaluation* eval)
{
    int value = exp->evaluation(eval);
    if(isDefined && !eval->isDefined(var))
            throw "Word "+var+" is not defined.";
    eval->setValue(var,value);
    eval->setNextLineNum(-1);
}

PrintStmt::PrintStmt(Tokenizer* s, Basic* b)
{
    tokenizer = s;
    basic = b;
    program = b->program;
    exp = b->parser->scanExpression(s);
    if (exp == nullptr)
        throw QString("Invalid expression after PRINT.");
    if (s->getLineNum() == -1 ){
        int val = exp->evaluation(b->eval);
        basic->write(QString::number(val));
    }
    program->setParsed(this);
}

void PrintStmt::execute(Evaluation* eval)
{
    int value = exp->evaluation(eval);
    basic->write(QString::number(value));
    eval->setNextLineNum(-1);
}

InputStmt::InputStmt(Tokenizer* s, Basic* b)
{
    tokenizer = s;
    basic = b;
    program = b->program;
    var = s->getNextToken();
    if(s->getTokenType(var) != WORD)
        throw QString("Invalid variable " + var);

    if(tokenizer->hasMoreTokens())
        throw QString("Extraneous tokens");
    program->setParsed(this);
}

void InputStmt::execute(Evaluation* eval)
{
    basic->write(var+" ? ");
    eval->setValue(var,getValue());
    basic->inputGotten();
    eval->setNextLineNum(-1);
}

void InputStmt::waitSec(int s)
{
    QTime t;
    t.start();
    while(t.elapsed()<s)
        QCoreApplication::processEvents();
}

int InputStmt::getValue()
{
    while(1){
        waitSec(10);
        if(basic->getInputValue()!=""){
            break;
        }
    }
    if (basic->getInputValue()!="INVALID"){
        int res =  basic->getInputValue().toInt();
        basic->inputGotten();
        return res;
    }else {
        basic->write("Ivalid input gotten! Please input again.");
        basic->write(var+" ? ");
        basic->inputGotten();
        return getValue();
    }
}

GotoStmt::GotoStmt(Tokenizer* s, Basic* b)
{
    tokenizer = s;
    basic = b;
    program = b->program;
    nextLineNum = s->getNextToken();
    if(s->getTokenType(nextLineNum)!=NUMBER)
        throw QString("The command after GOTO needs to be an integer line number");

    if(s->hasMoreTokens()){
        throw QString("Extraneous tokens");
    }
    program->setParsed(this);
}

void GotoStmt::execute(Evaluation* eval)
{
    eval->setNextLineNum(nextLineNum.toInt());
    return;
}

IfStmt::IfStmt(Tokenizer* s, Basic* b)
{
    tokenizer = s;
    basic = b;
    program = b->program;
    expLeft = b->parser->scanExpression(s);
    op = s->getNextToken();
    expRight = b->parser->scanExpression(s);
    if (!expLeft || op=="" ||!expRight)
        throw QString("Incorrect commands.");

    QString then = s->getNextToken();
    if (then!="THEN")
        throw QString("Missing command THEN.");
    nextLineNum = s->getNextToken();
    if(s->getTokenType(nextLineNum)!=NUMBER)
        throw QString("The command after THEN needs to be an integer line number");

    program->setParsed(this);
}

void IfStmt::execute(Evaluation* eval)
{
    if (checkCondition(eval))
        eval->setNextLineNum(nextLineNum.toInt());
    else
        eval->setNextLineNum(-1);
}

bool IfStmt::checkCondition(Evaluation* eval)
{
    if(op == "=") {
        if (int(expLeft->evaluation(eval)) == int(expRight->evaluation(eval)))
            return true;
    }
    if(op == ">") {
        if (expLeft->evaluation(eval) > expRight->evaluation(eval))
            return true;
    }
    if(op == "<") {
        if (expLeft->evaluation(eval) < expRight->evaluation(eval))
            return true;
    }
    return false;
}

EndStmt::EndStmt(Tokenizer* s, Basic* b)
{
    tokenizer = s;
    basic = b;
    program = b->program;

    if(!s->hasMoreTokens()){
        program->setParsed(this);
        return;
    }

    funcName = s->getNextToken();
    if(s->getTokenType(funcName)!=WORD)
        throw QString("Invalid funcation name!");

    if(s->hasMoreTokens())
        throw QString("Extraneous tokens");

    func_end = tokenizer->getLineNum();
    basic->eval->endFunction(funcName, func_end);
    program->setParsed(this);
}

void EndStmt::execute(Evaluation* eval)
{
    if(funcName=="")eval->setNextLineNum(-1);
    else{
        if(eval->isCalled(funcName)){
            eval->setUncalled(funcName);
            int returnLine = eval->getReturnLine();
            if (returnLine == -1)
                throw 0;
            eval->setNextLineNum(returnLine);          
        }else {
            eval->setNextLineNum(-1);
        }
    }
}

SubStmt::SubStmt(Tokenizer* s, Basic* b)
{
    tokenizer = s;
    basic = b;
    program = b->program;

    funcName = s->getNextToken();
    if(s->getTokenType(funcName)!=WORD)
        throw QString("Invalid funcation name!");

    func_start = s->getLineNum();
    if(s->hasMoreTokens())
        throw QString("Extraneous tokens");

    if(!basic->eval->isDefinedFunc(funcName))
        basic->eval->addFunction(funcName,func_start);
    else throw QString("Already defined function: "+funcName);

    program->setParsed(this);
}

void SubStmt::execute(Evaluation* eval)
{
    if(eval->isCalled(funcName))
        eval->setNextLineNum(-1);
    else
        eval->setNextLineNum(program->getNextLineNum(eval->getFunctionEnd(funcName)));
}

CallStmt::CallStmt(Tokenizer* s, Basic* b)
{
    tokenizer = s;
    basic = b;
    program = b->program;

    lineNum = tokenizer->getLineNum();
    funcName = s->getNextToken();
    if(s->getTokenType(funcName)!=WORD){
        throw QString("Invalid funcation name!");
    }
    if(s->hasMoreTokens()){
        throw QString("Extraneous tokens");
    }
    program->setParsed(this);
}

void CallStmt::execute(Evaluation* eval)
{
    if(!eval->isDefinedFunc(funcName))
        throw "Can not call Undefined function: "+funcName;
    eval->setCalled(funcName);
    eval->setReturnLine(program->getNextLineNum(lineNum));
    eval->setNextLineNum(eval->getFunctionStart(funcName));
}
