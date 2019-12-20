/*
 * File: expression.h
 * -----------
 * This file export class Expression and three concrete
 * subclasses: ConstantExp, IdentifierExp, CompoundExp.
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "evaluation.h"

/* Type of three Expression subclass */
enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };

/*
 * Class: Expression
 * -----------------
 * This class is used to represent a node in an expression tree.
 * Three concrete subclasses of Expression are used to represent
 * there different cases:
 *
 *  1. ConstantExp   -- an integer constant
 *  2. IdentifierExp -- a string representing an identifier
 *  3. CompoundExp   -- two expressions combined by an operator
 *
 */
class Expression
{
public:
    Expression();
    virtual ~Expression();

    /*
     * Method: evaluation
     * -----------------
     * Get the value of the expression.
     */
    virtual int evaluation(Evaluation *eval) = 0;

    /*
     * Method: getType
     * -----------------
     * Get the type of the expression.
     */
    virtual ExpressionType getType() = 0;
};

class ConstantExp: public Expression {
private:
    int value;
public:
    ConstantExp(int value);
    virtual int evaluation(Evaluation *eval);
    virtual ExpressionType getType(){return CONSTANT;}
};

class IdentifierExp: public Expression {
private:
    QString variable;
public:
    IdentifierExp(QString variable);
    virtual int evaluation(Evaluation *eval);
    QString getIdentifierName(){return variable;}
    virtual ExpressionType getType(){return IDENTIFIER;}
};

class CompoundExp: public Expression {
private:
    Expression* left;
    Expression* right;
    QString op;
public:
    CompoundExp(QString op, Expression* left, Expression* right);
    virtual int evaluation(Evaluation *eval);
    virtual ExpressionType getType(){return COMPOUND;}
};

#endif // EXPRESSION_H
