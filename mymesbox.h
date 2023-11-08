#ifndef MYMESBOX_H
#define MYMESBOX_H

#include <QMessageBox>

class myMesBox
{
public:
    myMesBox();
    myMesBox(QString title, QString text);
    void run();

private:
    QMessageBox mes;

};

#endif // MYMESBOX_H
