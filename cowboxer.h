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

    void sliceCurrentBox();
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
    void trimAndAddBox(int x, int y, int w, int h, QString str);


private:
    QPixmap pixmap;
    QImage  imgmap;
    boxList boxlist;
    int currentBox;
    bool modified;

public slots:
    void chopChar();
    void appendChar(QChar curChar);
    void clearString();

};
#endif // COWBOXER_H
