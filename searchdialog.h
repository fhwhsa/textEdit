#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QString>
#include <QPlainTextEdit>
#include <QTextCursor>
#include <QMessageBox>

namespace Ui {
class searchDialog;
}

class searchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit searchDialog(QWidget *parent = nullptr);
    explicit searchDialog(QPlainTextEdit* target);

    ~searchDialog();

private slots:
    void on_btnSearchNext_clicked();

private:
    Ui::searchDialog *ui;
    QPlainTextEdit *plainTextEdit;
    bool flag = false; // 协助控制向上/下查找

    void init();
    void iniSignalSlots();
};

#endif // SEARCHDIALOG_H
