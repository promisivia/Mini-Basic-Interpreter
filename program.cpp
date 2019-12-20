#include "program.h"

Program::Program()
{
}

Program::~Program()
{
}

void Program::clear()
{
    list.clear();
    map.clear();
}

int Program::getFirstLineNum()
{
    if(!list.isEmpty())
        return list.at(0)->lineNum;
    else return -1;
}

int Program::getNextLineNum(int currentLineNum)
{
    int location;
    for (location = 0; location < list.size()-1; ++location) {
        if(list.at(location)->lineNum == currentLineNum)
            return list.at(location+1)->lineNum;
    }
    return -1;
}

void Program::insertLine(int lineNum, QString text)
{
    Lines* tmp = new Lines(lineNum,text,nullptr);
    if(list.isEmpty())list.append(tmp);
    else{
        int location;
        for (location = 0; location < list.size(); ++location) {
            if(list.at(location)->lineNum>=lineNum)
                break;
        }
        if (location == list.size())
            list.insert(location,tmp);
        else if(list.at(location)->lineNum !=lineNum)
            list.insert(location,tmp);
        else list.replace(location,tmp);
    }
}

void Program::removeLine(int lineNum)
{
    for (int location = 0; location < list.size(); ++location) {
        if(list.at(location)->lineNum == lineNum){
            list.removeAt(location);
            map.remove(location);
        }
    }
}

void Program::setParsed(Statement *s)
{
    int lineNum = s->getLineNum();
    if (lineNum == -1) return;
    for (int location = 0; location < list.size(); ++location) {
        if(list[location]->stm == nullptr){
            if(list.at(location)->lineNum == lineNum){
                list[location]->stm = s;
                map.insertMulti(lineNum,list[location]);
            }else list.removeAt(location);
        }
    }
}

Statement* Program::getParsedStatement(int lineNum)
{
    if (map.contains(lineNum))
        return map[lineNum]->stm;
    else return nullptr;
}
