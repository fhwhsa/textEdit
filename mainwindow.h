#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QSize>
#include <QLabel>
#include <QTextCursor>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QTextCursor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel statusLabel;
    QLabel statusCursorLabel;
    QLabel author;
    bool isInitialState; // 标记程序是否刚启动，因为QPlainTextEdit初始时发送了一次textChanged信号
    bool isSaved;
    QString currFileName;
    QString copyContext;

    void init();
    void iniSignalSlots();

private slots:
    void setStatusBarText();

    void on_action_about_triggered();
    void on_action_find_triggered();
    void on_action_replace_triggered();
    void on_action_new_triggered();
    void on_action_open_triggered();
    void on_action_save_triggered();
    void on_action_saveAs_triggered();
    void on_plainTextEdit_textChanged();
    void on_action_cut_triggered();

signals:
    void copyAvailable(bool);
};
#endif // MAINWINDOW_H
