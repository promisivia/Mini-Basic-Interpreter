/*
 * File: program.h
 * ---------------
 * This file exports a Program class for storing the program.
 */

#ifndef PROGRAM_H
#define BROGRAM_H

#include <QString>
#include <QList>
#include "statement.h"

class Program
{
public:
    struct Lines
    {
       int lineNum = -1;
       QString statement;
       Statement *stm;
       Lines()=default;
       Lines(int li, QString stm, Statement *s):lineNum(li),statement(stm),stm(s){}
    };

    QList<Lines*> list;
    QMap<int,Lines*> map;

public slots:
    Program();
    ~Program();

    /*
     * Method: clear
     * -----------------------
     * Removes all lines from the program.
     */
    void clear();

    /*
     * Method: insertLine
     * ------------------------
     * Adds a line to the program with the specified line number.
     * If that line already exists, the text of the line replaces
     * the text of any existing line. If the line is new, it is
     * added to the program in the correct sequence.
     */
    void insertLine(int lineNum, QString text);

    /*
     * Method: removeLine
     * --------------------------------------------
     * Removes the line with the specified number from the program,
     * If no such line exists, this method does nothing.
     */
    void removeLine(int lineNum);

    /*
     * Method: getFirstLineNum
     * --------------------------------------------
     * Get the first line number of the whole program.
     */
    int getFirstLineNum();

    /*
     * Method: getNextLineNum
     * --------------------------------------------
     * Get next line number of the program according to the line
     * number now.
     */
    int getNextLineNum(int currentLineNum);

    /*
     * Method: setParsed
     * --------------------------------------------
     * Adds the parsed representation of the statement to the
     * statement at the specified line number.
     */
    void setParsed(Statement *s);

    /*
     * Method: getParsedStatement
     * --------------------------------------------
     * Get the parsed statement of the specified line number.
     */
    Statement* getParsedStatement(int lineNum);
};

#endif // BUFFER_H
