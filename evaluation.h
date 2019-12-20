/*
 * File: evaluation.h
 * -----------------
 * Export a class called Evaluation, which records the additional
 * information required by the evaluator(e.g. the declared functions
 * and variables) and changes the order of the execution of programs.
 */

#ifndef EVALUATION_H
#define EVALUATION_H

#include <QString>
#include <QMap>
#include <QStack>

/*
 * Struct: Func
 * ----------------
 * Stores the basic information of a function, where it starts, where
 * it ends and where should it return after it being called, whelter
 * it is being called.
 */
struct Func{
    int func_start;
    int func_end;
    bool CALLED = false;
    Func(int fs=-1, int fe=-1):func_start(fs), func_end(fe){}
};

/*
 * Class: Evaluation
 * ----------------
 * This class contains a varMap that maps variables into their
 * values, a funcMap that maps functions into their locations
 * and calling state. By record nextline, this class support the
 * features of GOTO and CALL.
 */
class Evaluation
{
private:  
    QMap<QString, int> varMap;
    QMap<QString, Func> funcMap;
    QStack<int> returnLine;
    int nextLine = -1;

public:
    Evaluation();

    /*
     * Method: clear
     * ----------------------------------
     * Clear the varMap and FuncMap.
     */
    void clear();

    /*
     * Method: setValue, getValue, isDefined
     * ----------------------------------
     * Set the value of the variable. Return its value. Check
     * whelther a variable is defined.
     */
    void setValue(QString variable, int value);
    int getValue(QString variable);
    bool isDefined(QString variable);

    /*
     * Method: setNextLineNum, getNextLineNum
     * ----------------------------------
     * Operate next line to control where the function go.
     */
    void setNextLineNum(int line);
    int getNextLineNum(){return nextLine;}

    /*
     * Method: addFunction .etc
     * ----------------------------------
     * Set or get the basic information of the function.
     */
    void addFunction(QString funcName, int func_start);
    bool isDefinedFunc(QString funcName);
    void endFunction(QString funcName, int func_end); 
    int getFunctionStart(QString funcName);
    int getFunctionEnd(QString funcName);

    void setReturnLine(int line);
    int getReturnLine();

    void setCalled(QString funcName){funcMap[funcName].CALLED = 1;}
    void setUncalled(QString funcName){funcMap[funcName].CALLED = 0;}
    bool isCalled(QString funcName){return funcMap[funcName].CALLED;}
};

#endif // EVALUATION_H
