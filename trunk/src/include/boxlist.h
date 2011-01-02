/***********************************************************************
** Copyright (C) 2010, 20011 Rakesh Achanta <rakeshvar at gmail.com>
** Copyright (C) 2010 Aleksey Sytchev <194145 at gmail.com>
**
** This file is part of Cowboser project
** http://code.google.com/p/cowboxer/
** All rights reserved.
**
** This file may be used under the terms of the GNU GENERAL PUBLIC
** LICENSE Version 3, 29 June 2007 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING
** THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
** PURPOSE.
**
***********************************************************************/

#ifndef BOXLIST_H
#define BOXLIST_H

#include <QObject>
#include <QRect>
#include <QList>

class boxList : public QObject
{
public:
boxList(QObject *parent = 0);
void clearAll();
void addBox(QRect newRect, QString newStr);
void addBox(int index, QRect newRect, QString newStr);
void addBoxOnly(int index, QRect newRect);
void addStrOnly(int index, QString newStr);
void removeBox(int index);
void removeBoxOnly(int index);
void removeStrOnly(int index);

QRect getRect(int index);
QString getStr(int index);
void setRect(int index, QRect newRect);
void setStr(int index, QString newStr);
void chopStr(int index);

int maxSize();
int minSize();
int boxSize();
int strSize();
bool isEmpty();
int getIndexByPos(QPoint point);

private:
QList<QString> strList;
QList<QRect> rectList;
};

#endif // BOXLIST_H
