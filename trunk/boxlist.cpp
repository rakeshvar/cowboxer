#include <stdlib.h>
#include "boxlist.h"

boxList::boxList(QObject *parent)
     : QObject(parent)
{
    rectList.clear();
    strList.clear();
}

void boxList::clearAll()
{
    rectList.clear();
    strList.clear();
}

void boxList::addBox(QRect newRect, QString newStr)
{
    rectList.append(newRect);
    strList.append(newStr);
}

void boxList::addBox(int index, QRect newRect, QString newStr)
{
    if(index > minSize()) 
        index = minSize(); 
    if(index < 0) 
        index = 0; 

    rectList.insert(index, newRect);
    strList.insert(index, newStr);
}

void boxList::addBoxOnly(int index, QRect newRect)
{
    if(index > boxSize()) 
        index = boxSize(); 
    if(index < 0) 
        index = 0; 

    rectList.insert(index, newRect);
}

void boxList::addStrOnly(int index, QString newStr)
{
    if(index > strSize()) 
        index = strSize(); 
    if(index < 0) 
        index = 0; 

    strList.insert(index, newStr);
}

QRect boxList::getRect(int index)
{
    if ((rectList.size() == 0) || (index < 0) || (index >= rectList.size()))
        return(QRect(0, 0, 0, 0));
    return(rectList.at(index));
}

QString boxList::getStr(int index)
{
    if ((rectList.size() == 0) || (index < 0) || (index >= strList.size()))
        return(QString("Error"));
    return(strList.at(index));
}

void boxList::removeBox(int index)
{
    if(rectList.size() == 0)
        return;
    rectList.removeAt(index);
    strList.removeAt(index);
}

void boxList::removeBoxOnly(int index)
{
    if(rectList.size() == 0)
        return;
    rectList.removeAt(index);
}

void boxList::removeStrOnly(int index)
{
    if(rectList.size() == 0)
        return;
    strList.removeAt(index);
}

void boxList::setRect(int index, QRect newRect)
{
    if(rectList.size() == 0)
        return;
    if(index >= rectList.size())
        return;
    rectList[index] = newRect;
}

void boxList::setStr(int index, QString newStr)
{
    if(strList.size() == 0)
        return;
    if(index >= strList.size())
        return;
    strList[index] = newStr;
}

void boxList::chopStr(int index)
{
    if(strList.size() == 0)
        return;
    if(index >= strList.size())
        return;
    strList[index].chop(1);
}

int boxList::boxSize()
{
    return(rectList.size());
}

int boxList::strSize()
{
    return(strList.size());
}

int boxList::maxSize()
{
    return (rectList.size()>strList.size())?rectList.size():strList.size();
}

int boxList::minSize()
{
    return (rectList.size()<strList.size())?rectList.size():strList.size();
}

bool boxList::isEmpty()
{
    if(maxSize() == 0)
        return true;
    return false;
}

int boxList::getIndexByPos(QPoint point)
{
    for(int i = 0; i < rectList.size(); ++i) {
        if(rectList.at(i).contains(point))
            return i;
    }
    unsigned int smallestDistance = (unsigned int)(-1); // big distance
    unsigned int curDistance = 0;
    int bestIndex = 0;
    for(int i = 0; i < rectList.size(); ++i) {
        curDistance = abs(rectList.at(i).center().x() - point.x()) + abs(rectList.at(i).center().y() - point.y());
        if(curDistance < smallestDistance) {
            bestIndex = i;
            smallestDistance = curDistance;
        }
    }
    return bestIndex;
}

