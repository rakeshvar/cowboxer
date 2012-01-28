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

#include <QtGui>
#include "cowboxer.h"
//#include <QDebug>

#define DETAIL_VIEW 2                   // Magnification for current symbol Detailed View Rectangle
#define TEXT_RECT_FRAC 3                // Size of text rectangle compared  to Detailed View Rectangle
#define ENABLE_BIG_TEXT_RECT 1          // Show or hide text rectangle inside Detailed View Rectangle

#define FAST_STEP 4
#define REPAINT_EMIT    repaint(); \
  emit currentBoxChanged(boxlist.getRect(currentBox).center().x(), boxlist.getRect(currentBox).center().y());

#define MOD_REPAINT_EMIT     modified = true; \
  REPAINT_EMIT


cowBoxer::cowBoxer(QWidget *parent) : QWidget(parent)
{
  pixmap.load(":/images/images/imageNotLoaded.png");
  modified = false;
  currentBox = 0;                   // set current box to 0
}

void cowBoxer::paintEvent(QPaintEvent * /* event */)
{
  // ! warning ! note that we works coordinate system, which differs from tesseract coordinate system.
  // (0,0) point is in bottom left corner of the widget
  QPainter painter(this);
  QPen pen;
  QBrush brush;

  painter.drawPixmap(0, 0, pixmap);

  for (int i = 0; i < boxlist.boxSize(); ++i)           // lets get all boxes
    {
      painter.setPen(Qt::blue);                         // set blue border to all boxes
      if (i == currentBox)
        painter.setPen(Qt::red);                        // set red border for the current box
      painter.drawRect(boxlist.getRect(i));             // draw the border box

      if ((boxlist.getStr(i) == "~") ||                 // Boxes with a ~ (Usually represents Error)
          (i % 20 == 0) ||                              // Every 20th Box
          (abs(currentBox - i) < 3))                    // And 5 boxes around the current
        {                                               // Have Text shown above the box
          QRect textRect;
          textRect = boxlist.getRect(i);
          textRect.moveBottom(textRect.top());
          painter.setFont(QFont("Arial", 12));
          painter.drawText(textRect, Qt::AlignBottom | Qt::AlignHCenter, boxlist.getStr(i));
        }
    }

  if (boxlist.isEmpty())
    return;

  // Draw the current box in DETAIL_VIEW*DETAIL_VIEW times the size
  QRect detailedView;
  int margin = 5;   // margin
  detailedView = boxlist.getRect(currentBox);
  detailedView.moveTop(detailedView.bottom() + margin);
  detailedView.setWidth(detailedView.width() * DETAIL_VIEW);
  detailedView.setHeight(detailedView.height() * DETAIL_VIEW);

  // Verify that our detailed veiw is in widget
  while (detailedView.bottom() > pixmap.height())
    detailedView.moveTop(detailedView.top() - 1);
  while (detailedView.top() < 0)
    detailedView.moveTop(detailedView.top() + 1);
  while (detailedView.right() > size().width())
    detailedView.moveLeft(detailedView.left() - 1);
  while (detailedView.left() < 0)
    detailedView.moveLeft(detailedView.left() + 1);

  // Tune brush & pen
  pen.setWidth(5);
  pen.setColor(Qt::black);
  brush.setStyle(Qt::SolidPattern);
  brush.setColor(Qt::white);
  painter.setBrush(brush);
  painter.setPen(pen);

  // Draw detailed rectangle
  painter.drawRect(detailedView);
  painter.drawPixmap(detailedView, pixmap, boxlist.getRect(currentBox));

#if ENABLE_BIG_TEXT_RECT
  // Make text rectangle based on detailedView
  QRect textRect;
  textRect = detailedView;
  textRect.setWidth(detailedView.width() / TEXT_RECT_FRAC);
  textRect.setHeight(detailedView.height() / TEXT_RECT_FRAC);
  textRect.moveCenter(detailedView.center());

  // Tune current char rectangle
  pen.setWidth(1);
  pen.setColor(Qt::darkRed);
  brush.setStyle(Qt::SolidPattern);
  brush.setColor(Qt::white);
  painter.setBrush(brush);
  painter.setPen(pen);
  painter.drawRoundRect(textRect, 40, 40);           // draw char rectangle

  // Set text style
  pen.setColor(Qt::darkRed);
  painter.setPen(pen);
  painter.setFont(QFont("Arial", 12, QFont::Bold));
  // Paint current character
  painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignHCenter, boxlist.getStr(currentBox));
#endif
}

bool cowBoxer::isEmpty()
{
  return boxlist.isEmpty();
}

bool cowBoxer::isModified()
{
  return modified;
}

void cowBoxer::setImageFile(const QString &imageFileName)
{
  // Dummy page no. 0. TODO: Multi-page TIF support using
  // QImageReader's imageCount and jumpToImage methods
  pixmap.load(imageFileName);   // yeh! thas all :)
  imgmap.load(imageFileName);
  resize(pixmap.size());
  repaint();
}

bool cowBoxer::loadBoxFile(const QString &boxlistFileName)
{
  modified = false;

  QFile file(boxlistFileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  boxlist.clearAll();   // clear all previous boxes
  QTextStream in(&file);
  in.setCodec("UTF-8");
  while (!in.atEnd())
    {
      QString line = in.readLine();
      processLine(line);   // add the box information from the line
    }
  currentBox = 0;   // set current box to 0

  if (boxlist.isEmpty())  // if list is empty try to do boxing
  {
      // Add one big box covering the entire image and then double slice it
      QRect newRect;
      newRect.setRect(0, 0, pixmap.width()-1, pixmap.height()-1);
      boxlist.addBox(newRect, (QString)"A");
      sliceDouble();
  }

  REPAINT_EMIT;
  return true;
}

bool cowBoxer::saveBoxFile(const QString &boxlistFileName)
{
  QFile file(boxlistFileName);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return false;

  int x1, x2, y1, y2;   //coords
  QTextStream out(&file);
  out.setCodec("UTF-8");
  for (int i = 0; i < boxlist.maxSize(); ++i)
    {
      boxlist.getRect(i).getCoords(&x1, &y1, &x2, &y2);
      out << boxlist.getStr(i) << ' '
          << x1 << ' '
          << pixmap.height() - y2 << ' '
          << x2 << ' '
          << pixmap.height() - y1 << ' '
          << '0';
                // Dummy page no. 0. TODO: Multi-page TIF support using
                // QImageReader's imageCount and jumpToImage methods

      // if this is not the last element, we should start next line
      if (i < boxlist.maxSize() - 1)
        out << '\n';
    }
  modified = false;
  return true;
}

void cowBoxer::processLine(QString line)
{
  QString curStr;

  curStr = line.section(' ', 0, 0);   // from 0 indexed symbol to 0 indexed symbol
  QRect curRect;
  curRect.setCoords(line.section(' ', 1, 1).toInt(),   // x1
                    pixmap.height() - line.section(' ', 4, 4).toInt(),   // y1
                    line.section(' ', 3, 3).toInt(),   // x2
                    pixmap.height() - line.section(' ', 2, 2).toInt());  // y2
  boxlist.addBox(curRect, curStr);   // add the box to the list

  if (curStr == "~")                    // Correct the bad boxes generated by tesseract
    {
      currentBox = boxlist.minSize() - 1;
      sliceCurrentBoxVert();
    }
}

void cowBoxer::moveUp(bool fast)
{
  int step = 1;

  if (fast)
    step = FAST_STEP;
  QRect changed = boxlist.getRect(currentBox);
  changed.moveBottom(boxlist.getRect(currentBox).bottom() - step);
  boxlist.setRect(currentBox, changed);

  MOD_REPAINT_EMIT;
}

void cowBoxer::moveDown(bool fast)
{
  int step = 1;

  if (fast)
    step = FAST_STEP;
  QRect changed = boxlist.getRect(currentBox);
  changed.moveBottom(boxlist.getRect(currentBox).bottom() + step);
  boxlist.setRect(currentBox, changed);

  MOD_REPAINT_EMIT;
}

void cowBoxer::moveLeft(bool fast)
{
  int step = 1;

  if (fast)
    step = FAST_STEP;
  QRect changed = boxlist.getRect(currentBox);
  changed.moveLeft(boxlist.getRect(currentBox).left() - step);
  boxlist.setRect(currentBox, changed);

  MOD_REPAINT_EMIT;
}

void cowBoxer::moveRight(bool fast)
{
  int step = 1;

  if (fast)
    step = FAST_STEP;
  QRect changed = boxlist.getRect(currentBox);
  changed.moveLeft(boxlist.getRect(currentBox).left() + step);
  boxlist.setRect(currentBox, changed);

  MOD_REPAINT_EMIT;
}

void cowBoxer::chopChar()
{
  boxlist.chopStr(currentBox);
  MOD_REPAINT_EMIT;
}

void cowBoxer::appendChar(QChar curChar)
{
  boxlist.setStr(currentBox, boxlist.getStr(currentBox).append(curChar));
  MOD_REPAINT_EMIT;
}

void cowBoxer::clearString()
{
  boxlist.setStr(currentBox, "");
  MOD_REPAINT_EMIT;
}

void cowBoxer::nextBox()
{
  if (boxlist.isEmpty())  // if list is empty - do nothing
    return;

  if (currentBox == boxlist.boxSize() - 1)  // the last element is current
    currentBox = 0;
  else
    ++currentBox;
  REPAINT_EMIT;
}

void cowBoxer::prevBox()
{
  if (boxlist.isEmpty())  // if list is empty - do nothing
    return;

  if (currentBox == 0)  // the first element is current
    currentBox = boxlist.boxSize() - 1;
  else
    --currentBox;
  REPAINT_EMIT;
}

void cowBoxer::increaseHeight(bool fast)
{
  int step = 1;

  if (fast)
    step = FAST_STEP;
  QRect changed = boxlist.getRect(currentBox);
  changed.setHeight(boxlist.getRect(currentBox).height() + step);
  boxlist.setRect(currentBox, changed);

  MOD_REPAINT_EMIT;
}

void cowBoxer::decreaseHeight(bool fast)
{
  int step = 1;

  if (fast)
    step = FAST_STEP;
  QRect changed = boxlist.getRect(currentBox);
  if (boxlist.getRect(currentBox).height() - step > 0)   // can we decrease height?
    changed.setHeight(boxlist.getRect(currentBox).height() - step);
  boxlist.setRect(currentBox, changed);

  MOD_REPAINT_EMIT;
}

void cowBoxer::increaseWidth(bool fast)
{
  int step = 1;

  if (fast)
    step = FAST_STEP;
  QRect changed = boxlist.getRect(currentBox);
  changed.setWidth(boxlist.getRect(currentBox).width() + step);
  boxlist.setRect(currentBox, changed);

  MOD_REPAINT_EMIT;
}

void cowBoxer::decreaseWidth(bool fast)
{
  int step = 1;

  if (fast)
    step = FAST_STEP;
  QRect changed = boxlist.getRect(currentBox);
  if (boxlist.getRect(currentBox).width() - step > 0)  // can we decrease height?
    changed.setWidth(boxlist.getRect(currentBox).width() - step);
  boxlist.setRect(currentBox, changed);

  MOD_REPAINT_EMIT;
}

void cowBoxer::removeCurrentBox()
{
  if (boxlist.isEmpty())  // if list is empty - do nothing
    return;
  if (currentBox <= boxlist.minSize())
    {
      boxlist.removeBox(currentBox);
      if (currentBox == boxlist.minSize())    // if the last element was deleted
        --currentBox;     // set the new last element
      MOD_REPAINT_EMIT;
    }
}

void cowBoxer::removeStrOnly()
{
  if (boxlist.isEmpty())  // if list is empty - do nothing
    return;
  if (currentBox <= boxlist.strSize())
    {
      boxlist.removeStrOnly(currentBox);
      MOD_REPAINT_EMIT;
    }
}

void cowBoxer::removeBoxOnly()
{
  if (boxlist.isEmpty())  // if list is empty - do nothing
    return;
  if (currentBox <= boxlist.boxSize())
    {
      boxlist.removeBoxOnly(currentBox);
      if (currentBox == boxlist.boxSize())    // if the last element was deleted
        --currentBox;     // set the new last element
      MOD_REPAINT_EMIT;
    }
}

void cowBoxer::insertBox()
{
  QRect newRect;

  if (boxlist.isEmpty())    // if list is empty - imagine new rect
    {
      currentBox = 0;
      newRect.setRect(20, 80, 20, 40);
      boxlist.addBox(newRect, (QString)"A");
    }
  else
    {
      newRect = boxlist.getRect(currentBox);
      newRect.moveRight(newRect.right() + 20);
      boxlist.addBox(++currentBox, newRect, (QString)"A");
    }
  MOD_REPAINT_EMIT;
}

void cowBoxer::insertBoxOnly()
{
  QRect newRect;

  if (boxlist.boxSize() == 0)    // if list is empty - imagine new rect
    {
      currentBox = 0;
      newRect.setRect(20, 80, 20, 40);
      boxlist.addBoxOnly(0, newRect);
    }
  else
    {
      newRect = boxlist.getRect(currentBox);
      newRect.moveRight(newRect.right() + 20);
      boxlist.addBoxOnly(++currentBox, newRect);
    }
  MOD_REPAINT_EMIT;
}

void cowBoxer::insertStrOnly()
{
  if (boxlist.strSize() == 0)    // if list is empty - imagine new rect
    {
      boxlist.addStrOnly(0, (QString)"New");
    }
  else
    {
      boxlist.addStrOnly(currentBox, (QString)"A");
    }
  MOD_REPAINT_EMIT;
}

void cowBoxer::mousePressEvent(QMouseEvent *event)
{
  if (boxlist.isEmpty())  // if list is empty - do nothing
    return;

  QPoint translation;
  translation = event->pos();
  //translation.setY(pixmap.height() - translation.y());
  if (event->button() == Qt::LeftButton)
    {
      currentBox = boxlist.getIndexByPos(translation);
      REPAINT_EMIT;
    }
}


bool cowBoxer::isHWhite(int x1, int x2, int y)
{
  // Checks to see if an entire line from (x1,y) to (x2, y) is White
  for (int i = x1; i <= x2; i++)
    if (imgmap.pixel(i, y) != qRgb(255, 255, 255))
      return false;
  return true;
}

bool cowBoxer::isVWhite(int x, int y1, int y2)
{
  // Checks to see if an entire line from (x,y1) to (x, y2) is White
  for (int i = y1; i <= y2; i++)
    if (imgmap.pixel(x, i) != qRgb(255, 255, 255))
      return false;
  return true;
}

void cowBoxer::sliceCurrentBoxVert()
{
  // Slices a given box vertically to generate smaller boxes and trims them.
  // Called on boxes with a ~ in them.
  // Also invoked via the Tab button.

  bool closed = true;           // New box opens when a vertical nonwhite line is found
  int x1, x2, y1, y2;
  int xclose, xopen = 1;        // Dummy assignment to supress warning

  QString str = boxlist.getStr(currentBox);

  boxlist.getRect(currentBox).getCoords(&x1, &y1, &x2, &y2);
  boxlist.removeBox(currentBox);            // Remove the old box and make fresh ones.

  for (int x = x1; x <= x2; x++)
    {
      if (closed)
        {   // New box is opened when a vertical nonwhite line is found
          if (!isVWhite(x, y1, y2))
            {
              xopen = x;
              closed = false;
            }
        }
      else
        {   // An open box is closed when a vertical white line is found
          if (isVWhite(x, y1, y2))
            {
              xclose = x;
              closed = true;
              trimTopBottomAndAddBox(xopen, y1, xclose - xopen, y2 - y1 + 1, str);
            }
        }
    }
  if (!closed)
    trimTopBottomAndAddBox(xopen, y1, x2 - xopen + 1, y2 - y1 + 1, str);

  if (currentBox > 0)
    currentBox--;                     // Set Current Box to the last created box

  MOD_REPAINT_EMIT;
}

void cowBoxer::sliceCurrentBoxHoriz()
{
    // Slices a given box horizontally to generate smaller boxes
    //
    // Invoked via pressing Control + Tab

    bool closed = true;           // New box opens when a vertical nonwhite line is found
    int x1, x2, y1, y2;
    int yclose, yopen = 1;        // Dummy assignment to supress warning

    QString str = boxlist.getStr(currentBox);

    boxlist.getRect(currentBox).getCoords(&x1, &y1, &x2, &y2);
    boxlist.removeBox(currentBox);            // Remove the old box and make fresh ones.

    for (int y = y1; y <= y2; y++)
      {
        if (closed)
          {   // New box is opened when a vertical nonwhite line is found
            if (!isHWhite(x1, x2, y))
              {
                yopen = y;
                closed = false;
              }
          }
        else
          {   // An open box is closed when a vertical white line is found
            if (isHWhite(x1, x2, y))
              {
                yclose = y;
                closed = true;
                trimSidesAndAddBox(x1, yopen, x2 - x1 + 1, yclose - yopen, str);
              }
          }
      }
    if (!closed)
      trimSidesAndAddBox(x1, yopen, x2 - x1 + 1, y2 - yopen + 1, str);

    if (currentBox > 0)
      currentBox--;                     // Set Current Box to the last created box

    MOD_REPAINT_EMIT;

}

void cowBoxer::sliceDouble()
{
    int initiallyAt = currentBox;
    sliceCurrentBoxHoriz();
    int boxesAdded = currentBox - initiallyAt;

    if (boxesAdded == 0)
        return;
    currentBox = initiallyAt-1;

    for(int iBox = 0; iBox <= boxesAdded; iBox++)
    {
        currentBox++;
        sliceCurrentBoxVert();
    }
}

void cowBoxer::trimTopBottomAndAddBox(int x, int y, int w, int h, QString str)
{
  // Trims a box to remove white space at the top and bottom
  // before adding it to the list of boxes
  while (isHWhite(x, x + w - 1, y))
    {
      y++; h--;
    }
  while (isHWhite(x, x + w - 1, y + h - 1))
    {
      h--;
    }

  boxlist.addBox(currentBox, QRect(x, y, w, h), str);
  ++currentBox;
}

void cowBoxer::trimSidesAndAddBox(int x, int y, int w, int h, QString str)
{
  // Trims a box to remove white space at the right and left
  // before adding it to the list of boxes
  while (isVWhite(x, y, y + h - 1))
    {
      x++; w--;
    }
  while (isVWhite(x + w - 1, y, y + h - 1))
    {
      w--;
    }

  boxlist.addBox(currentBox, QRect(x, y, w, h), str);
  ++currentBox;
}
