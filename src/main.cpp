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

#include <QtGui/QApplication>
#include <QtCore/QtPlugin>
#include "mainwindow.h"

#ifdef WIN32
Q_IMPORT_PLUGIN(qtiff)
#endif

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MainWindow w;

  //try to open first parameter input file
  if (argc == 2)
        w.openBox(argv[1]);

  w.show();
  return app.exec();
}
