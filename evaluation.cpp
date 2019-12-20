#include "evaluation.h"
#include "basic.h"

Evaluation::Evaluation()
{
}

void Evaluation::clear()
{
    varMap.clear();
    funcMap.clear();
    nextLine = -1;
}

void Evaluation::setValue(QString variable, int value)
{
    if (varMap.contains(variable))
        varMap[variable] = value;
    else varMap.insertMulti(variable, value);
}

int Evaluation::getValue(QString variable)
{
    return varMap[variable];
}

bool Evaluation::isDefined(QString variable)
{
    return varMap.contains(variable);
}

void Evaluation::setNextLineNum(int line)
{
    this->nextLine = line;
}

bool Evaluation::isDefinedFunc(QString funcName)
{
    return funcMap.contains(funcName);
}

void Evaluation::addFunction(QString funcName, int func_start)
{
    funcMap.insertMulti(funcName,Func(func_start,-1));
}

void Evaluation::endFunction(QString funcName, int func_end){
    funcMap[funcName].func_end = func_end;
}

void Evaluation::setReturnLine(int line)
{
    returnLine.push(line);
}

int Evaluation::getFunctionStart(QString funcName)
{
    return funcMap[funcName].func_start;
}

int Evaluation::getFunctionEnd(QString funcName)
{
    return funcMap[funcName].func_end;
}

int Evaluation::getReturnLine()
{
    return returnLine.pop();
}
