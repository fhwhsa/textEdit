#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSize>
#include <QLabel>
#include <QTextCursor>
#include <QString>

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

    void init();
    void iniSignalSlots();

private slots:
    void setStatusBarText();
};
#endif // MAINWINDOW_H
