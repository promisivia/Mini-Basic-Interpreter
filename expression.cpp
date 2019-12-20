#include "expression.h"

Expression::Expression()
{

}

Expression::~Expression()
{

}

ConstantExp::ConstantExp(int value)
{
    this->value = value;
}

int ConstantExp::evaluation(Evaluation *eval)
{
    return value;
}

IdentifierExp::IdentifierExp(QString variable)
{
    this->variable = variable;
}

int IdentifierExp::evaluation(Evaluation *eval)
{
    if (!eval->isDefined(variable))
        throw "Undefined variable:"+variable;
    return eval->getValue(variable);
}

CompoundExp::CompoundExp(QString op, Expression* left, Expression* right)
{
    this->op = op;
    this->left = left;
    this->right = right;
}

int CompoundExp::evaluation(Evaluation *eval)
{
    if (op == "=") {
       if (right->getType() != IDENTIFIER) {
          throw QString("Illegal variable in assignment");
       }
       int val = right->evaluation(eval);
       eval->setValue(static_cast<IdentifierExp *>(left)->getIdentifierName(), val);
       return val;
    }
    int l = left->evaluation(eval);
    int r = right->evaluation(eval);
    if (op == "+") return l + r;
    if (op == "-") return l - r;
    if (op == "*") return l * r;
    if (op == "/") return l / r;
    throw QString("Illegal operator in expression");
}
