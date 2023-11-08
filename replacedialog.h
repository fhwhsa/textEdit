#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QString>
#include <QPlainTextEdit>
#include <QTextCursor>

namespace Ui {
class replaceDialog;
}

class replaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit replaceDialog(QWidget *parent = nullptr);
    explicit replaceDialog(QPlainTextEdit* target);
    ~replaceDialog();

private slots:
    void on_btnSearchNext_clicked();

    void on_btnReplace_clicked();

    void on_btnReplaceAll_clicked();

private:
    Ui::replaceDialog *ui;
    QPlainTextEdit* plainTextEdit;
    int l, r; // 当前查找选择的子字符串
    bool flag = false; // 协助控制向上/下查找

    void init();
    void iniSignalSlots();
};

#endif // REPLACEDIALOG_H
