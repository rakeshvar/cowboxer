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
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  init();
  setCurrentFile(tr(""));
}

MainWindow::~MainWindow()
{
}

MainWindow::MainWindow(const QString &fileName)
{
  init();
  loadFile(fileName);
}

void MainWindow::init()
{
  isUntitled = true;

  cowboxer = new cowBoxer(this);
  cowboxer->setMinimumSize(253, 133);   // size of "image not loaded" image

  scrollArea = new cowScrollArea;
  scrollArea->setBackgroundRole(QPalette::Base);
  scrollArea->setWidget(cowboxer);
  scrollArea->setCowboxer(cowboxer);

  setCentralWidget(scrollArea);
  //setWindowIcon(QIcon(":/images/images/icon.png"));
  setWindowIcon(QIcon(":/images/images/cowboxer.svg"));

  setActions();
  readSettings();   //set the window to the last opened position
}

void MainWindow::setActions()
{
  // action to create new box file
  newAct = new QAction(tr("&New"), this);
  newAct->setIcon(QIcon(":/images/images/new.png"));
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("Create a new box file"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

  // action to open existing box file
  openBoxAct = new QAction(tr("&Open box file..."), this);
  openBoxAct->setIcon(QIcon(":/images/images/openBox.png"));
  openBoxAct->setShortcuts(QKeySequence::Open);
  openBoxAct->setStatusTip(tr("Open an existing box file"));
  connect(openBoxAct, SIGNAL(triggered()), this, SLOT(openBox()));

  // action to save box file
  saveBoxAct = new QAction(tr("&Save box file"), this);
  saveBoxAct->setIcon(QIcon(":/images/images/save.png"));
  saveBoxAct->setShortcuts(QKeySequence::Save);
  saveBoxAct->setStatusTip(tr("Save the box file"));
  connect(saveBoxAct, SIGNAL(triggered()), this, SLOT(save()));

  // action to save box file
  saveAsBoxAct = new QAction(tr("Save box file &as..."), this);
  saveAsBoxAct->setIcon(QIcon(":/images/images/save-as.png"));
  saveAsBoxAct->setShortcuts(QKeySequence::SaveAs);
  saveAsBoxAct->setStatusTip(tr("Save the box file as..."));
  connect(saveAsBoxAct, SIGNAL(triggered()), this, SLOT(saveAs()));

  // action to open image, that concurs to the box file
  openImageAct = new QAction(tr("Open &image file..."), this);
  openImageAct->setIcon(QIcon(":/images/images/openImage.png"));
  openImageAct->setShortcuts(QKeySequence::Italic);   // Italic means alias of Ctrl+I in all systems. Actually there is no any actions with font type.
  openImageAct->setStatusTip(tr("Open an image file"));
  connect(openImageAct, SIGNAL(triggered()), this, SLOT(openImage()));

  // about act
  aboutAct = new QAction(tr("&About"), this);
  aboutAct->setIcon(QIcon(":/images/images/help.png"));
  aboutAct->setShortcuts(QKeySequence::HelpContents);
  aboutAct->setStatusTip(tr("How to use CowBoxer"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct = new QAction(tr("About &Qt"), this);
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  // action exit
  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setStatusTip(tr("Shut down the program"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  // add actions to the file menu
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(openBoxAct);
  fileMenu->addAction(saveBoxAct);
  fileMenu->addAction(saveAsBoxAct);
  fileMenu->addAction(openImageAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  // add actions to the about menu
  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);

  // add actions to the toolbars
  fileToolBar = new QToolBar(tr("File"));
  fileToolBar->addAction(newAct);
  fileToolBar->addAction(openBoxAct);
  fileToolBar->addAction(saveBoxAct);
  fileToolBar->addAction(saveAsBoxAct);
  fileToolBar->addAction(openImageAct);
  fileToolBar->addAction(aboutAct);
  addToolBar(fileToolBar);

  connect(cowboxer, SIGNAL(currentBoxChanged(int, int)),
          scrollArea, SLOT(ensurePositionVisibility(int, int)));
}

void MainWindow::openBox()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Tesseract box file"),
                                                  tr(""),
                                                  tr("Tesseract box files (*.box);;All files (*.*)"));

  if (!fileName.isEmpty())
    {
      MainWindow *existing = findMainWindow(fileName);
      if (existing)     // if this file already opened
        {
          existing->show();
          existing->raise();
          existing->activateWindow();
          return;
        }
      if (isUntitled && cowboxer->isEmpty() && !isWindowModified())     // if there is no opened document before
        {
          loadFile(fileName);
        }
      else       // if we have opened document then we create the new one
        {
          MainWindow *other;
          other = new MainWindow(fileName);
          if (other->isUntitled)     // something goes wrong? delete
            {
              delete other;
              return;
            }
          other->move(x() + 40, y() + 40);
          other->show();
        }
    }
}

void MainWindow::openImage()
{
  QString imageFileName = QFileDialog::getOpenFileName(NULL, tr("Open image file"),
                                                       tr(""),
                                                       tr("Tiff files (*.tif *.tiff);;Image files (*.bmp *.png *.jpeg *.jpg *.tif *.tiff);;All files (*.*)"));

  if (!imageFileName.isEmpty())     // if we have file
    {
      cowboxer->setImageFile(imageFileName);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if (maybeSave())
    {
      writeSettings();
      event->accept();
    }
  else
    {
      event->ignore();
    }
}

void MainWindow::newFile()
{
  MainWindow *other = new MainWindow;

  other->move(x() + 40, y() + 40);
  other->show();
}

bool MainWindow::save()
{
  if (isUntitled)
    {
      return saveAs();
    }
  else
    {
      return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), curFile,
                                                  tr("Tesseract box files (*.box);;All files (*.*)"));

  if (fileName.isEmpty())
    return false;

  return saveFile(fileName);
}

void MainWindow::about()
{
  QMessageBox::about(this, tr("About CowBoxer"),
                     tr(
                       "<h1>CowBoxer 1.01</h1>"
                       "The <b>CowBoxer</b> is the tool for training the \"Google Tesseract OCR\" "
                       "<h2>Controls</h2>"
                       "<p><b>Arrow key</b> - Move box</p>"
                       "<p><b>Arrow key + Shift</b> - Fast moving</p>"
                       "<p><b>Arrow key + Ctrl</b> - Change box size</p>"
                       "<p><b>Arrow key + Ctrl + Shift</b> - Fast box size changing</p>"
                       "<p><b>Return</b> - Select next box</p>"
                       "<p><b>Shift + Return</b> - Select previous box</p>"
                       "<p><b>Ctrl + Shift + Del</b> - Delete current box along with the string</p>"
                       "<p><b>Ctrl + Del</b> - Delete current box only (string transfers to next box)</p>"
                       "<p><b>Shift + Del</b> - Delete current box's string only (next box's string transfers here)</p>"
                       "<p><b>Del</b> - Clear current box's string</p>"
                       "<p><b>Ctrl + Shift + Ins</b> - Insert new box & string (\"A\")</p>"
                       "<p><b>Ctrl + Ins</b> - Insert new box only (Next box's string moves to this box)</p>"
                       "<p><b>Shift + Ins</b> - Insert new string only (This string moves to next box)</p>"
                       "<p><b>Tab</b> - Slice the current Box into sub boxes if possible and trim them. On loading a box file, boxes containing a \"~\" are sliced automatically.</p>"
                       "<p><b>Backspace</b> - Backspace String</p>"
                       "<p><b>Any key</b> - Input string for the current box</p>"
                       "<h2>Project page</h2>"
                       "<p><a href=\"http://code.google.com/p/cowboxer/\">CowBoxer homepage at Google Code</a></p>"
                       "<p>CowBoxer is available under the <a href=\"http://www.gnu.org/licenses/gpl.html\">GNU General Public License v3</a></p>"
                       "<h2>Author(s)</h2>"
                       "<p><b>Aleksey Sytchev</b> - 194145 (at) gmail.com</p>"
                       "<p><b>Rakeshvara Rao</b> - rakeshvar (at) gmail.com</p>"));
}

void MainWindow::readSettings()
{
  QSettings settings("Aleksey Sytchev", "CowBoxer");
  QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
  QSize size = settings.value("size", QSize(400, 400)).toSize();

  move(pos);
  resize(size);
}

void MainWindow::writeSettings()
{
  QSettings settings("Aleksey Sytchev", "CowBoxer");

  settings.setValue("pos", pos());
  settings.setValue("size", size());
}

bool MainWindow::maybeSave()
{
  if (cowboxer->isModified())
    {
      QMessageBox::StandardButton ret;
      ret = QMessageBox::warning(this, tr("CowBoxer"), tr("The boxfile has been modified.\n"
                                                          "Do you want to save your changes?"),
                                 QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      if (ret == QMessageBox::Save)
        return save();
      else if (ret == QMessageBox::Cancel)
        return false;
    }
  return true;
}

void MainWindow::loadFile(const QString &fileName)
{
  QFile file(fileName);

  if (!file.open(QFile::ReadOnly | QFile::Text))
    {
      QMessageBox::warning(this, tr("CowBoxer"),
                           tr("Cannot read file %1:\n%2.")
                           .arg(fileName)
                           .arg(file.errorString()));
      return;
    }
  QString imageFileName = file.fileName();   // remember this path to acces to the image file
  file.close();   // [I don't need problems with you. Get off! You are useless now. ] I'll call you later, maybe. Bye.


  // try to load .tif file with the same path
  imageFileName.replace(imageFileName.lastIndexOf(tr(".box")), 4, ".tif");   // change the file extension
  QFileInfo imageFileInfo(imageFileName);
  if (imageFileInfo.exists())
    {
      cowboxer->setImageFile(imageFileName);   // we have got it! load and run out of function
    }
  else
    {
      // oh so bad! maybe our file have .tiff extension.
      imageFileName.replace(imageFileName.lastIndexOf(tr(".tif")), 4, ".tiff");   // change the file extension
      imageFileInfo.setFile(imageFileName);
      if (imageFileInfo.exists())    //
        {
          cowboxer->setImageFile(imageFileName);   // thank's God. Take it and run out of this terrible function
        }
      else
        openImage();            // What!? Does image file not loaded yet? Get me its name, and i'll get it!
    }

  QApplication::setOverrideCursor(Qt::WaitCursor);
  bool loadedSuccessful = cowboxer->loadBoxFile(fileName);
  QApplication::restoreOverrideCursor();
  if (!loadedSuccessful)    // if i can't read the file
    {
      QMessageBox::warning(this, tr("CowBoxer"),
                           tr("Cannot read file %1:\nAre you sure that you don't read the binary file?")
                           .arg(fileName));
      return;
    }
  isUntitled = false;
  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
  QFile file(fileName);

  if (!file.open(QFile::WriteOnly | QFile::Text))
    {
      QMessageBox::warning(this, tr("CowBoxer"),
                           tr("Cannot write file %1:\n%2.")
                           .arg(fileName)
                           .arg(file.errorString()));
      return false;
    }

  QApplication::setOverrideCursor(Qt::WaitCursor);
  bool savedSuccessful = cowboxer->saveBoxFile(fileName);
  QApplication::restoreOverrideCursor();
  if (!savedSuccessful)    // if i can't save the file
    {
      QMessageBox::warning(this, tr("CowBoxer"),
                           tr("Cannot save file %1:\nThat's bad :( Maybe you want to rewrite the readonly file.")
                           .arg(fileName));
      return false;
    }

  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File saved"), 2000);
  isUntitled = false;
  return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
  static int sequenceNumber = 1;

  isUntitled = fileName.isEmpty();
  if (isUntitled)
    {
      curFile = tr("box%1.box").arg(sequenceNumber++);
    }
  else
    {
      curFile = QFileInfo(fileName).canonicalFilePath();
    }

  setWindowTitle(tr("%1 - %2").arg(strippedName(curFile))
                 .arg(tr("CowBoxer")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}

MainWindow *MainWindow::findMainWindow(const QString &fileName)
{
  QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

  foreach(QWidget * widget, qApp->topLevelWidgets())
  {
    MainWindow *mainWin = qobject_cast<MainWindow *>(widget);

    if (mainWin && mainWin->curFile == canonicalFilePath)
      return mainWin;
  }
  return 0;
}
