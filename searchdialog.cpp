#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QDebug>

searchDialog::searchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::searchDialog)
{
    ui->setupUi(this);
}

searchDialog::searchDialog(QPlainTextEdit* target) :
    ui(new Ui::searchDialog)
{
    plainTextEdit = target;
    ui->setupUi(this);
    init();
    iniSignalSlots();
}

searchDialog::~searchDialog()
{
    delete ui;
}

void searchDialog::init()
{
    ui->chIsCase->setChecked(true);
    ui->radioDown->setChecked(true);
}

void searchDialog::iniSignalSlots()
{
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
}

void searchDialog::on_btnSearchNext_clicked()
{
    QString target = ui->searchTarget->text();
    QString context = plainTextEdit->toPlainText();
    bool isUp = ui->radioUp->isChecked();
    Qt::CaseSensitivity type = ui->chIsCase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;

    if (target.size() == 0 || context.size() == 0) {
        QMessageBox::warning(this, "Waining", "Target or context is empty!");
        return;
    }
    if (!context.contains(target, type)) {
        QMessageBox::warning(this, "Warning", "Not found!");
        return;
    }

    QTextCursor cursor = plainTextEdit->textCursor();
    int pos = cursor.position(), len = target.size(), t;

    if (isUp) t = context.lastIndexOf(target, pos - (flag ? len + 1 : 0), type);
    else t = context.indexOf(target, pos, type);
    if (t >= 0)
    {
        cursor.setPosition(t, QTextCursor::MoveAnchor); // 定位起点
        cursor.setPosition(t + len, QTextCursor::KeepAnchor); // 终点
        plainTextEdit->setTextCursor(cursor);
        if (isUp) flag = 1;
        else flag = 0;
    }
    else
    {
        cursor.setPosition(isUp ? (int)context.size() : 0);
        flag = 0;
        plainTextEdit->setTextCursor(cursor);
        ui->btnSearchNext->click();
    }
}
