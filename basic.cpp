#include "basic.h"

#include <QKeyEvent>
#include <QTextLine>
#include <QTextCursor>
#include <QDebug>
#include <QFont>

Basic::Basic(QWidget *parent) : QTextEdit(parent)
{
    program = new Program;
    tokenizer = new Tokenizer;
    parser = new Parser;
    eval = new Evaluation();
    this->setFont(QFont("Consolas",14));
    this->write("MINIMAL BASIC -- Type HELP for help.\n");
    resize(800,600);
}

void Basic::write(QString string)
{
    this->append(string);
}

QString Basic::getInputValue()
{
    if (runtimeInput == "")
        return "";
    tokenizer->scanLine(runtimeInput);
    QString token = tokenizer->getNextToken();
    token = tokenizer->getNextToken();
    QString value = tokenizer->getNextToken();
    if(tokenizer->getTokenType(value)!= NUMBER || tokenizer->hasMoreTokens()){
        return "INVALID";
    }
    return value;
}

void Basic::inputGotten(){
    runtimeInput = "";
}

void Basic::processLine(QString line)
{
        tokenizer->scanLine(line);
        if (state == RUNNING){
            runtimeInput = line;
        }else{
            QString firstTerm = tokenizer->getNextToken();
            if(tokenizer->getTokenType(firstTerm) != NUMBER)
                processCommand(firstTerm);
            else
                processCode(firstTerm.toInt(),line);
        }

}

void Basic::processCode(int lineNum, QString line)
{
    if (lineNum <= 0)
        throw QString("Line number should be greater than 0!");
    if (tokenizer->hasMoreTokens()){
        program->insertLine(lineNum, line);
        parser->parseStatement(tokenizer,this);
    } else {
        if(program->map.contains(lineNum))
            program->removeLine(lineNum);
        else throw QString("No such line to remove!");
    }
}

void Basic::processCommand(QString cmd)
{
   QString CMD = cmd.toUpper();
   if(CMD == "RUN") {
       run();
   }else if(CMD == "LIST") {
       listProgram();
   } else if(CMD == "CLEAR") {
       clear();
   } else if(CMD == "HELP") {
       printHelpMsg();
   } else if(CMD == "QUIT") {
       exit(0);
   } else if(CMD == "PRINT") {
       tokenizer->trackback();
       parser->parseStatement(tokenizer,this);
   } else {
       throw "Invalid COMMAND: " + CMD + ". Type HELP for help.";
   }
}

void Basic::run()
{
    state = RUNNING;
    int index = program->getFirstLineNum();
    while(index != -1){
        try{
            program->getParsedStatement(index)->execute(eval);
        }catch(int){
            break;
        }catch(const QString error) {
            this->write(error);
            break;
        }
        if (eval->getNextLineNum() != -1){
            index = eval->getNextLineNum();
        }else {
            index = program->getNextLineNum(index);
        }
    }
    state = NOTRUNNING;
}

void Basic::listProgram()
{
    for (int i = 0; i < program->list.size(); ++i) {
        if(program->list.at(i)->stm)
            this->write(program->list.at(i)->statement);
    }
}

void Basic::clear()
{
    program->clear();
    eval->clear();
}


void Basic::printHelpMsg()
{
    this->write("The following commands are accepted by the interpreter:");
    this->write("RUN - Runs the stored program.");
    this->write("LIST - Lists the stored program.");
    this->write("CLEAR - Deletes the stored program.");
    this->write("HELP - Displays help information.");
    this->write("QUIT - Exits the interpreter.\n");

    this->write("The following statements are accepted by the interpreter:");
    this->write("The LET, PRINT, and INPUT statements can be executed directly by typing them without a line number.");
    this->write("Following commands cannot be part of a program and must therefore be entered without a line number. ");
    this->write("[Usage: REM exp]: This statement is used for comments. Any text on the line after the keyword REM is ignored.");
    this->write("[Usage: LET var = exp]: Assignment statement. Assigns the value of the expression to the variable, replacing any previous value.");
    this->write("[Usage: PRINT exp]: Prints value of the expression to the console.");
    this->write("[Usage: INPUT var]: Reads in a variable from the user. Prompts the user by printing \" ? \", and assigns the input value to the variable");
    this->write("[Usage: GOTO n]: Forces program to execute line n instead of the next stored line.");
    this->write("[Usage: IF exp1 op exp2 THEN n]*: Conditional operator op accepts =, <, and > to compare exp1 and exp2. If condition holds, executes line n. If not, program executes the next stored line.");
    this->write("[Usage: END]: Halts program execution.\n");

    this->write("Functions are supported with the following statements.");
    this->write("[Usage: SUB function_name, Statements, End Sub ]: the function can use variables defined outside the function, and variables defined inside the function can also be accessed in other parts of the code.");
    this->write("[Usage: CALL function_name]:  invoked the function.\n");
}

void Basic::keyPressEvent(QKeyEvent *event)
{
    QTextCursor cursor = this->textCursor();
    cursor.movePosition(QTextCursor::End);
    this->setTextCursor(cursor);
    ensureCursorVisible();

    if (event->key() == Qt::Key_Backspace){
        cursor.deletePreviousChar();
        return;
    }
    if (event->key() == Qt::Key_Delete){
        cursor.deleteChar();
        return;
    }
    if (this->textCursor().hasSelection())
        return;
    if (event->key() == Qt::Key_Return) {
        cursor.select(QTextCursor::LineUnderCursor);
        QString Line = cursor.selectedText();
        try{
            processLine(Line);
        }catch (const QString error) {
            this->write(error);
        }
    }
    QTextEdit::keyPressEvent(event);
}
