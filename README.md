# MINIBASIC

A tiny but powerful interpreter that allows a user to code, save and run programs in BASIC. 

## 1. Overview

### 1.1 Supported usage
* Required `COMMAND` in the document and add support for function.
* Immediately print the comstant expression and expression only contain constant number and defined varivables.
  > HINT: Each token should split by a blankspace.
  > -1 can either prepresent as -1 or - 1.
  > support expression like 1 * - 2
* SYNTAX ERROR will be printed on the console during writing program. RUNTIME ERROR will be printed and stop the running at once.
  
### 1.2 Files
* **basic**: Export a QTextEdit class to implement a console to interact with the user. Process the command and print information.
* **tokenizer**: Provide support to scan strings and divide it into a list of tokens.
* **parser**: Provide support to scan statements (create different statements according to the command scaned) and to scan expressions (create a binary expression tree according to the tokens scanned).
* **statement**: Export class Statement and its concrete subclasses.
* **expression**: Export class Expression and its three concrete subclasses: ConstantExp, IdentifierExp, CompoundExp.
* **evaluation**: Export a class called Evaluation. A space storing all additional information required by the evaluator(e.g. the declared functions and variables). Act like %rip, we use it to change the execution order of programs.

## 2. Main Procedure

### 2.1 process the input line

* The `keyPressEvent` function receives the line input by the user and trigger the `processLine` function.
* If this line begins with a number, it trigger `parseStatement` function and store it in the program.
* Else, it trigger the `processCommand` function to process the command.
### 2.2 process the code

* First store the program, then use `parseStatement` function exported in the `parser` class to parse this line.
* According to the command, the `parseStatement` function new different Subclass of `Statement`.
* Each constructor of `Statement` check the expression of the program and stored the information (like the variable name, the GOTO line number) it needed when being executed. If the expression is valid, it call `setParsed` function implemented by `program.h` file to decide whelter to remove this line of code.

### 2.3 process the command

* According to the command, the `processCommand` function call different functions.
* `listProgram` print the lines stored in program out on the window, `clear` delete all the previous information and `printHelpMsg` print all the help information.
* `run` has the following structures:
    ```
    int index = first line number of this program
    while(index != -1){
        execute the statment
        if (eval->getNextLineNum() != -1){
            index = eval->getNextLineNum();
        }else {
            index = program->getNextLineNum(index);
        }
    }   
    ```
&emsp;&emsp;eval is used to change the line of the program to be executed, which supports the GOTO and IF THEN command. 

### 2.4 details of how `execute()` work

#### 2.4.1 evaluation
* `Expression` class create a Expression tree, each node is a `IdentifierExp` class, a `CompoundExp` class or a `ConstantExp` class.
* the evalution method use recursive algorithm to return the value of a expression tree.
* the value of variables are stored in `evaluation` class.

#### 2.4.2 input
* Use RUNNING and UNRUNNING flag in console to mark whelter the program is running. When running, if get a value inputted, store it.
* Check whelter the value is inputted every 0.1s by implementing `waitSec` method.
* Check the validity of the input, if it is invalid, repeat again.

#### 2.4.3 function
* Store the function name, their starting line and end line in `evaluation` class.
* When meet `CALL` command, push the return line in the stack. When reach the end of the function again, pop the line and set next line to it.

## 3. Error Handling


1. process SYNTAX ERROR

      ```
        try{
            processLine(Line);
        }catch (const QString error) {
            this->write(error);
        }
2. process RUNTIME ERROR
     ```
        try{
            getParsedStatement(index)->execute(eval);
        }catch(int){
            break;
        }catch(const QString error) {
            this->write(error);
            break;
        }
