#include "mymesbox.h"

myMesBox::myMesBox()
{

}

myMesBox::myMesBox(QString title, QString text)
{
    mes.setWindowTitle(title);
    mes.setText(text);
    run();
}

void myMesBox::run()
{
    mes.exec();
}
