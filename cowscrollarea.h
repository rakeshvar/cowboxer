#ifndef COWSCROLLAREA_H
#define COWSCROLLAREA_H

#include "cowboxer.h"
#include <QKeyEvent>
#include <QScrollArea>

class cowScrollArea : public QScrollArea
{
    Q_OBJECT
protected:
    void keyReleaseEvent ( QKeyEvent *  );
    void keyPressEvent ( QKeyEvent *  );

public:
    cowScrollArea();
    void setCowboxer(cowBoxer*);

private:
    cowBoxer* cowboxer;

public slots:
    void ensurePositionVisibility(int, int);
};

#endif // COWSCROLLAREA_H
