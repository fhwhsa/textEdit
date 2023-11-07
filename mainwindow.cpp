#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    this->resize(QSize(1200, 700));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    author.setText(tr("苏焯明"));
    author.setMaximumWidth(150);
    statusLabel.setMaximumWidth(150);
    statusCursorLabel.setMaximumWidth(150);

    setStatusBarText();
    iniSignalSlots();
}

void MainWindow::iniSignalSlots()
{
    connect(ui->plainTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(setStatusBarText()));
}

void MainWindow::setStatusBarText()
{
    qDebug() << 1;
    QTextCursor tc = ui->plainTextEdit->textCursor();

    ui->statusbar->clearMessage();

    statusLabel.setText(QString("length: %1   lines: %2|").arg(ui->plainTextEdit->toPlainText().size()).arg(tc.blockNumber() + 1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setText("Ln: " + QString::number(tc.blockNumber() + 1) + "   Col: " + QString::number(tc.columnNumber()) + "|");
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    ui->statusbar->addPermanentWidget(&author);
}
