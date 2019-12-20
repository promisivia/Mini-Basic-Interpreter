#include "tokenizer.h"

Tokenizer::Tokenizer()
{

}

void Tokenizer::scanLine(QString line)
{
    TokenList = line.split(" ");
    scanLocation = 0;

    /* check validity */
    for (int i = 0; i < TokenList.size(); ++i){
        if (getTokenType(TokenList.at(i))==STRING)
            throw "Invalid Input: "+TokenList.at(i)+". Each token should split by a blankspace.";
    }


    TokenType pre, now = getTokenType(TokenList[0]);
    /* process with the input like "1 * - 2" */
    for (int i = 0; i < TokenList.size(); ++i) {
        pre = now;
        now = getTokenType(TokenList[i]);
        if (pre == OPERATOR && now == OPERATOR){
            if (TokenList[i] == "-" && (TokenList[i] == "+"||TokenList[i] == "-"||TokenList[i] == "*"||TokenList[i] == "/") && getTokenType(TokenList[i+1]) == NUMBER){
                if (i == TokenList.size()-1 ) throw QString("Invalid expression after PRINT.");
                QString newToken = "-"+TokenList[i+1];
                TokenList.insert(i+1,newToken);
                TokenList.removeAt(i);
                TokenList.removeAt(i+1);
            }
        }
        /* process with the input like "-1 -2" */
        if (pre == NUMBER && now == NUMBER){
            if (TokenList[i].at(0) == "-"){
                QString newToken = TokenList[i].remove(0,1);
                TokenList.insert(i+1,"-");
                TokenList.insert(i+2,newToken);
                TokenList.removeAt(i);
            }
        }
    }
}

void Tokenizer::trackback()
{
    scanLocation--;
}

int Tokenizer::getLineNum()
{
    if (getTokenType( TokenList[0])==NUMBER)
        return TokenList[0].toInt();
    else return -1;
}

QString Tokenizer::getNextToken()
{
    QString token = "";
    if(scanLocation < TokenList.size()){
        token = TokenList.at(scanLocation);
        scanLocation++;
    }
    return token;
}

bool Tokenizer::hasMoreTokens()
{
    if(getNextToken()!=""){
        scanLocation--;
        return true;
    }else return false;
}

TokenType  Tokenizer::getTokenType(QString token){
    if(token == "") return EMPTY;
    if(reservedWord.contains(token)) return RESERVEREDWORD;
    if(validOp.contains(token)) return OPERATOR;

    bool NUMflag = true;
    if (token.at(0)!="-" && (token.at(0)<"0"||token.at(0)>"9"))
        NUMflag = false;
    for (int i=1; i<token.size(); ++i){
        if(token.at(i)<"0"||token.at(i)>"9")
            NUMflag = false;
    }
    if(NUMflag) return NUMBER;

    bool WORDflag = true;
    if(token.at(0).toUpper()<"A"||token.at(0).toUpper()>"Z")
         WORDflag = false;
    for (int i=1; i<token.size(); ++i){
        if( !((token.at(i)>="0"&&token.at(i)<="9")||(token.at(i).toUpper()>="A"&&token.at(i).toUpper()<="Z")))
         WORDflag = false;
    }
    if(WORDflag) return WORD;

    if(token.size()==1)
        throw QString("Invalid operate: "+token);

    return STRING;
}
