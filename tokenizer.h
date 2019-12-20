/*
 * File: Tokenizer.h
 * --------------
 * This file provides support to scan codes and divide it into tokens.
 */

#ifndef Tokenizer_H
#define Tokenizer_H

#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QSet>

/* enum : Token Type */
enum TokenType {WORD, RESERVEREDWORD, NUMBER, OPERATOR, STRING, EMPTY};

class Tokenizer
{
private:
    QStringList TokenList;
    QSet<QString> validOp { "+", "-" , "*" , "/", "(" , ")" , "=", ">", "<" };
    QSet<QString> reservedWord { "THEN", "GOTO" , "LET" , "RUN" };
    int scanLocation;

public slots:
    Tokenizer();

    /*
     * Method: scanLine
     * --------------
     * Scan a line, split the line into tokens by blankspace
     * and stores the tokens in the TokenList.
     */
    void scanLine(QString line);

    /*
     * Method: trackback
     * --------------
     * Go back to scan the previous token.
     */
    void trackback();

    /*
     * Method: getLineNum
     * --------------
     * Get the line nummber of the code this Tokenizer is scanning.
     * If the code does not begin with a line number, return -1.
     */
    int getLineNum();

    /*
     * Method: hasMoreTokens
     * --------------
     * Check whelter there are more tokens.
     */
    bool hasMoreTokens();

    /*
     * Method: getNextToken
     * --------------
     * Return next token in the form of QString.
     */
    QString getNextToken();


    /*
     * Method: getTokenType
     * --------------
     * Return the type of the token.
     */
    TokenType getTokenType(QString token);
};

#endif // Tokenizer_H
