/*
 * File: Basic.h
 * -------------------
 * Export a QTextEdit class to implement a console to interact
 * with the user.Process the command and print information.
 *
 */

#ifndef BASIC_H
#define BASIC_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

#include "program.h"
#include "tokenizer.h"
#include "parser.h"

/* Mark wheter the program is running */
enum State{RUNNING,NOTRUNNING};

class Basic : public QTextEdit
{
    Q_OBJECT

public:
    Program* program;
    Tokenizer* tokenizer;
    Parser* parser;
    Evaluation *eval;

private:
    QString runtimeInput;
    State state;

public:
    explicit Basic(QWidget *parent = nullptr);

    /*
     * Function: write
     * Usage: basic->write(QString);
     * -----------------------------------------
     * Print the string on the widget.
     */
    void write(QString string);

    /*
     * Function: processLine
     * -----------------------------------------
     * Get a single line entered by the user and decide if it
     * is a command or a line of code. Process accordingly.
     */
    void processLine(QString line);

    /*
     * Function: processCode
     * -----------------------------------------
     * Process the line with a line number with the help of parser.
     * Store it in the program.
     * If line only has a line number, delete it if this line already
     * exsits, or print that this line does not exists.
     */
    void processCode(int lineNum, QString line);

    /*
     * Function: processCommand
     * -----------------------------------------
     * Get a single command and checks its validity.
     * If valid, carries out the corresponding operation.
     */
    void processCommand(QString cmd);

    /*
     * Function: getInputValue
     * -----------------------------------------
     * Get the user input during the running of the program.
     */
    QString getInputValue();

    /*
     * Function: inputGotten
     * -----------------------------------------
     * Mark that the input is gotten by the evaluation.
     */
    void inputGotten();

    /*
     * Function: run
     * -----------------------------------------
     * Start executing the program.
     */
    void run();

    /*
     * Function: listProgram
     * -----------------------------------------------
     * Print the line stored in program out on the window.
     */
    void listProgram();

    /*
     * Function: clear
     * -----------------------------------------------
     * Delete all the previous information.
     */
    void clear();

    /*
     * Function: printHelpMsg
     * -----------------------------------------------
     * Support the PRINT command. Print all the help information.
     */
    void printHelpMsg();
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};


#endif // BASIC_H
