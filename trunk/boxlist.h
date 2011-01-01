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
