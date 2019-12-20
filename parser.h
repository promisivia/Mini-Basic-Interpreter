/*
 * File: parser.h
 * --------------
 * This file provides support to parse a code.
 */

#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "statement.h"
#include "expression.h"

#include <QStack>


class Parser
{
    QStack<QString> op;
    QStack<Expression*> exp;

public:
    Parser();
    /*
     * Function: parseStatement
     * -------------------------------------------
     * Create different statement according to the command scaned.
     */
    Statement* parseStatement(Tokenizer* tokenizer, Basic *basic);

    /*
     * Function: scanExpression
     * -------------------------------------------
     * Create a binary expression tree according to the tokens scanned
     * by the Tokenizer. Return the first node of the tree.
     */
    Expression* scanExpression(Tokenizer* tokenizer);

    /*
     * Function: getPrecedence
     * -------------------------------------------
     * Return the precedence of certain operate.
     */
    int getPrecedence(QString op);
};

#endif // PARSER_H
