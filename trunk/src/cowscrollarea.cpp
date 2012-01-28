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

#include "cowscrollarea.h"

cowScrollArea::cowScrollArea()
{
}

void cowScrollArea::setCowboxer(cowBoxer* newpoint)
{
  if (newpoint)
    cowboxer = newpoint;
}

void cowScrollArea::keyReleaseEvent(QKeyEvent * event)
{
  if (!cowboxer)  // if pointer to the cowboxer is not set -- do nothing
    return;
  switch (event->key())     // prevent scroll bar moving on arrow key pressing
    {
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
      return;

    default:
      QWidget::keyReleaseEvent(event);
    }
}

void cowScrollArea::keyPressEvent(QKeyEvent * event)
{
  if (!cowboxer)  // if pointer to the cowboxer is not set -- do nothing
    return;
  switch (event->key())
    {
    case Qt::Key_Left:
      if (event->modifiers() & Qt::ControlModifier)
        cowboxer->decreaseWidth(event->modifiers() & Qt::ShiftModifier);
      else
        cowboxer->moveLeft(event->modifiers() & Qt::ShiftModifier);
      break;

    case Qt::Key_Right:
      if (event->modifiers() & Qt::ControlModifier)
        cowboxer->increaseWidth(event->modifiers() & Qt::ShiftModifier);
      else
        cowboxer->moveRight(event->modifiers() & Qt::ShiftModifier);
      break;

    case Qt::Key_Up:
      if (event->modifiers() & Qt::ControlModifier)
        cowboxer->decreaseHeight(event->modifiers() & Qt::ShiftModifier);
      else
        cowboxer->moveUp(event->modifiers() & Qt::ShiftModifier);
      break;

    case Qt::Key_Down:
      if (event->modifiers() & Qt::ControlModifier)
        cowboxer->increaseHeight(event->modifiers() & Qt::ShiftModifier);
      else
        cowboxer->moveDown(event->modifiers() & Qt::ShiftModifier);
      break;

    case Qt::Key_Delete:
      if ((event->modifiers() & Qt::ControlModifier) && (event->modifiers() & Qt::ShiftModifier))
        cowboxer->removeCurrentBox();
      else if (event->modifiers() & Qt::ControlModifier)
        cowboxer->removeBoxOnly();
      else if (event->modifiers() & Qt::ShiftModifier)
        cowboxer->removeStrOnly();
      else
        cowboxer->clearString();
      break;

    case Qt::Key_Insert:
      if ((event->modifiers() & Qt::ControlModifier) && (event->modifiers() & Qt::ShiftModifier))
        cowboxer->insertBox();
      else if (event->modifiers() & Qt::ControlModifier)
        cowboxer->insertBoxOnly();
      else if (event->modifiers() & Qt::ShiftModifier)
        cowboxer->insertStrOnly();
      break;

    case Qt::Key_Tab:
      if (event->modifiers() & Qt::ControlModifier)
        cowboxer->sliceCurrentBoxHoriz();
      else
        cowboxer->sliceCurrentBoxVert();
      break;

    case Qt::Key_Home:
      cowboxer->sliceDouble();
      break;

    case Qt::Key_Enter:
    case Qt::Key_Return:
      if (event->modifiers() & Qt::ShiftModifier)
        cowboxer->prevBox();
      else
        cowboxer->nextBox();
      break;

    case Qt::Key_Backspace:
      cowboxer->chopChar();
      break;

    default:
      if (event->text().size() > 0)
        cowboxer->appendChar(event->text().at(0));
      break;
    }
}

void cowScrollArea::ensurePositionVisibility(int x, int y)
{
  ensureVisible(x, y, 200, 200);
}
