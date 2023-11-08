#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    iniSignalSlots();
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

    currFileName = "";
    isInitialState = true;
    isSaved = true;

    this->setWindowTitle("新建文件");
    setStatusBarText();
}

void MainWindow::iniSignalSlots()
{
    connect(ui->plainTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(setStatusBarText()));
}

void MainWindow::setStatusBarText()
{
    QTextCursor tc = ui->plainTextEdit->textCursor();

    ui->statusbar->clearMessage();

    statusLabel.setText(QString("length: %1   lines: %2|").arg(ui->plainTextEdit->toPlainText().size()).arg(tc.blockNumber() + 1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setText("Ln: " + QString::number(tc.blockNumber() + 1) + "   Col: " + QString::number(tc.columnNumber()) + "|");
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    ui->statusbar->addPermanentWidget(&author);
}

void MainWindow::on_action_about_triggered()
{
    AboutDialog dia;
    dia.exec();
}


void MainWindow::on_action_find_triggered()
{
    searchDialog dia(ui->plainTextEdit);
    dia.exec();
}


void MainWindow::on_action_replace_triggered()
{
    replaceDialog dia(ui->plainTextEdit);
    dia.exec();
}


void MainWindow::on_action_new_triggered()
{
    if (!isSaved) {
        QMessageBox::StandardButton result;
        result = QMessageBox::question(this, "Warning", QString("The file %1 has not been saved. Do you want to save it?").arg(currFileName));
        if (result == QMessageBox::Yes) {
            qDebug() << "Yes";
            if (currFileName != "") {
                emit ui->action_save->triggered();
            }
            else
                emit ui->action_saveAs->triggered();
        }
    }

    ui->plainTextEdit->clear(); // 发射textChanged信号，isSaved必须后面修改

    isSaved = true;
    currFileName = "";
    this->setWindowTitle("新建文件");
}


void MainWindow::on_action_open_triggered()
{
    if (!isSaved) {
        QMessageBox::StandardButton result;
        result = QMessageBox::question(this, "Warning", QString("The file %1 has not been saved. Do you want to save it?").arg(currFileName));
        if (result == QMessageBox::Yes) {
            if (currFileName != "") {
                emit ui->action_save->triggered();
            }
            else
                emit ui->action_saveAs->triggered();
        }
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("文件选取"), "/", tr("Text files(*.txt);;All files(*.*)"));
    if (fileName == "") return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "file open failed!");
        return;
    }

    QTextStream tStream(&file);
    tStream.setAutoDetectUnicode(true);
    QString context = tStream.readAll();
    ui->plainTextEdit->setPlainText(context);
    file.close();

    isSaved = true;
    currFileName = fileName;
    this->setWindowTitle(currFileName);
}


void MainWindow::on_action_save_triggered()
{
    if (currFileName != "") {
        QFile file(currFileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "file save failed!");
            return;
        }

        QTextStream tStream(&file);
        tStream.setAutoDetectUnicode(true);
        QString context = ui->plainTextEdit->toPlainText();
        tStream << context;
        file.flush();
        file.close();
        isSaved = true;
        this->setWindowTitle(currFileName);
        return;
    }
    else
        emit ui->action_saveAs->triggered();
}


void MainWindow::on_action_saveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("选择存储位置"), "/", tr("Text files(*.txt);;All files(*.*)"));
    if (fileName == "") return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "file save failed!");
        return;
    }

    QTextStream tStream(&file);
    tStream.setAutoDetectUnicode(true);
    QString context = ui->plainTextEdit->toPlainText();
    tStream << context;
    file.flush();
    file.close();

    isSaved = true;
    this->setWindowTitle(currFileName);
    currFileName = fileName;
}


void MainWindow::on_plainTextEdit_textChanged()
{
    if (isInitialState) {
        isInitialState = false;
        return;
    }
    isSaved = false;
    this->setWindowTitle("*" + (currFileName == "" ? "新建文件" : currFileName));
}

