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

#include <QPainter>
#include <QWidget>
#include <QTransform>
#include "boxlist.h"

#ifndef COWBOXER_H
#define COWBOXER_H


class cowBoxer : public QWidget
{
Q_OBJECT
public:
cowBoxer(QWidget *parent = 0);
bool isEmpty();
bool isModified();
void setImageFile(const QString &imageFileName);
bool loadBoxFile(const QString &boxlistFileName);
bool saveBoxFile(const QString &boxlistFileName);
void processLine(QString line);

void nextBox();
void prevBox();
void moveUp(bool fast = false);
void moveDown(bool fast = false);
void moveLeft(bool fast = false);
void moveRight(bool fast = false);
void increaseHeight(bool fast = false);
void decreaseHeight(bool fast = false);
void increaseWidth(bool fast = false);
void decreaseWidth(bool fast = false);

void sliceCurrentBoxVert();
void sliceCurrentBoxHoriz();
void sliceDouble();
void removeCurrentBox();
void removeStrOnly();
void removeBoxOnly();
void insertBox();
void insertBoxOnly();
void insertStrOnly();

signals:
void currentBoxChanged(int x, int y);
protected:
void paintEvent(QPaintEvent *event);
void mousePressEvent(QMouseEvent *event);

bool isHWhite(int x1, int x2, int y);
bool isVWhite(int x, int y1, int y2);
void trimTopBottomAndAddBox(int x, int y, int w, int h, QString str);
void trimSidesAndAddBox(int x, int y, int w, int h, QString str);

private:
QPixmap pixmap;
QImage imgmap;
boxList boxlist;
int currentBox;
bool modified;

public slots:
void chopChar();
void appendChar(QChar curChar);
void clearString();
};
#endif // COWBOXER_H
