#include "replacedialog.h"
#include "mymesbox.h"
#include "ui_replacedialog.h"

replaceDialog::replaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::replaceDialog)
{
    ui->setupUi(this);
}

replaceDialog::replaceDialog(QPlainTextEdit* target) :
    ui(new Ui::replaceDialog)
{
    plainTextEdit = target;
    ui->setupUi(this);
    init();
    iniSignalSlots();
}

replaceDialog::~replaceDialog()
{
    delete ui;
}

void replaceDialog::init()
{
    ui->chIsCase->setChecked(true);
    ui->radioDown->setChecked(true);
    l = -1; r = -1;
}

void replaceDialog::iniSignalSlots()
{
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
}

void replaceDialog::on_btnSearchNext_clicked()
{
    QString target = ui->searchTarget->text();
    QString context = plainTextEdit->toPlainText();
    bool isUp = ui->radioUp->isChecked();
    Qt::CaseSensitivity type = ui->chIsCase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;

    if (target.size() == 0 || context.size() == 0) {
        myMesBox mes("Error", "Target or context is empty!");
        return;
    }
    if (!context.contains(target)) {
        myMesBox mes("Warnning", "Not found!");
        return;
    }

    QTextCursor cursor = plainTextEdit->textCursor();
    int pos = cursor.position(), len = target.size(), t;

    if (isUp) t = context.lastIndexOf(target, pos - (flag ? len + 1 : 0), type);
    else t = context.indexOf(target, pos, type)
            ;
    if (t >= 0)
    {
        l = t; r = t + len;
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


void replaceDialog::on_btnReplace_clicked()
{
    QString replaceContext = ui->replaceContext->text();
    int len = replaceContext.size();

    if (l == -1) {
        myMesBox mes("Error", "No selected text!");
        return;
    }

    QString context = plainTextEdit->toPlainText();
    context.replace(l, r-l, replaceContext);
    plainTextEdit->setPlainText(context);

    // 重新选择
    QTextCursor cursor = plainTextEdit->textCursor();
    cursor.setPosition(l, QTextCursor::MoveAnchor); // 定位起点
    cursor.setPosition(l + len, QTextCursor::KeepAnchor); // 终点
    plainTextEdit->setTextCursor(cursor);
    r = l + len;
}


void replaceDialog::on_btnReplaceAll_clicked()
{
    QString replaceContext = ui->replaceContext->text();
    QString context = plainTextEdit->toPlainText();
    QString target = ui->searchTarget->text();

    if (target.size() == 0) {
        myMesBox mes("Error", "Target is empty!");
        return;
    }

    context.replace(target, replaceContext);
    plainTextEdit->setPlainText(context);
}

