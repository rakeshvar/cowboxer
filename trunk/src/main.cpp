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

#include <QFile>
#include <QLocale>
#include <QTranslator>
#include <QApplication>
#include <QtCore/QtPlugin>
#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QtGui>

#if defined(STATIC_LINK)
#include <QtPlugin>
Q_IMPORT_PLUGIN(qtiff)
#endif

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  app.setOrganizationName("Aleksey Sytchev");
  app.setApplicationName("CowBoxer");
  
  //check if translation directory exists
  QString m_path;
  QString appdir = QCoreApplication::applicationDirPath();
  QStringList paths;
  paths.append(appdir + "/translations/");
  paths.append(appdir + "/../translations/");
  paths.append(appdir + "/../share/" + QCoreApplication::applicationName().toLower() + "/translations/");
  paths.append("/usr/share/cowboxer/translations/");
  paths.append(appdir + "/../resource/translations");
  qDebug() << "*****____ appdir:" << appdir;
  foreach (const QString& path, paths) {
    qDebug() << "*****____ does it exists??? " << path;
    if (QFile::exists(path)) {
      m_path = path;
      break;
    }
  }

  // load translation
  QString locale = QLocale::system().name();
  locale.truncate(locale.lastIndexOf('_'));
  QTranslator translator;
  qDebug() << "*****____ locale:" << locale;
  translator.load("cowboxer_" + locale, m_path);
  app.installTranslator(&translator);

  MainWindow w;

  //try to open first parameter as input file
  if (argc == 2)
        w.openBox(argv[1]);

  w.show();
  return app.exec();
}
