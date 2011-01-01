#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include <QToolBar>

#include "cowboxer.h"
#include "cowscrollarea.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    MainWindow(const QString &fileName);
    ~MainWindow();
    bool isUntitled;

private:
    void init();
    void setActions();
    cowBoxer* cowboxer;
    QAction* newAct;
    QAction* openBoxAct;
    QAction* saveBoxAct;
    QAction* saveAsBoxAct;
    QAction* openImageAct;
    QAction* aboutAct;
    QAction* exitAct;
    QMenu* fileMenu;
    QMenu* helpMenu ;
    QToolBar* fileToolBar;
    void closeEvent(QCloseEvent *event);
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    MainWindow* findMainWindow(const QString &fileName);
    QString curFile;
    cowScrollArea* scrollArea;

private slots:
    void openBox();
    void openImage();
    void newFile();
    bool save();
    bool saveAs();
    void about();
};

#endif // MAINWINDOW_H
