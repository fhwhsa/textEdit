#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include "codeeditor.h"
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
    plainTextEdit = new QPlainTextEdit;
    this->setCentralWidget(plainTextEdit);

    author.setText(tr("苏焯明"));
    author.setMaximumWidth(150);
    statusLabel.setMaximumWidth(150);
    statusCursorLabel.setMaximumWidth(150);

    currFileName = "";
    isInitialState = true;
    isSaved = true;

    this->setWindowTitle("新建文件");
    setStatusBarText();

    ui->action_revoke->setEnabled(false);
    ui->action_recovery->setEnabled(false);
    ui->action_copy->setEnabled(false);
    ui->action_cut->setEnabled(false);

    ui->action_toolBar->setChecked(true);
    ui->action_statusbar->setChecked(true);
    ui->action_autoWrap->setChecked(true);
    ui->action_showLineNum->setChecked(false);
}

void MainWindow::iniSignalSlots()
{
    connect(plainTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(setStatusBarText()));

    connect(ui->action_revoke, SIGNAL(triggered()), plainTextEdit, SLOT(undo()));
    connect(plainTextEdit, &QPlainTextEdit::undoAvailable, [this](bool f) { ui->action_revoke->setEnabled(f); });

    connect(ui->action_recovery, SIGNAL(triggered()), plainTextEdit, SLOT(redo()));
    connect(plainTextEdit, &QPlainTextEdit::redoAvailable, [this](bool f) { ui->action_recovery->setEnabled(f); });

    connect(ui->action_copy, SIGNAL(triggered()), plainTextEdit, SLOT(copy()));
    connect(plainTextEdit, &QPlainTextEdit::copyAvailable, [this](bool f) {
        ui->action_copy->setEnabled(f);
        emit copyAvailable(f);
    });

    connect(ui->action_cut, SIGNAL(triggered()), plainTextEdit, SLOT(cut()));
    connect(plainTextEdit, &QPlainTextEdit::copyAvailable, [this](bool f) {ui->action_cut->setEnabled(f);});

    connect(ui->action_paste, SIGNAL(triggered()), plainTextEdit, SLOT(paste()));
    connect(ui->action_selectAll, SIGNAL(triggered()), plainTextEdit, SLOT(selectAll()));

    connect(ui->action_exit, SIGNAL(triggered()), this, SLOT(close()));
    connect(plainTextEdit, SIGNAL(textChanged()), this, SLOT(plainTextEdit_textChanged()));
}

void MainWindow::setStatusBarText()
{
    QTextCursor tc = plainTextEdit->textCursor();

    ui->statusbar->clearMessage();

    statusLabel.setText(QString("length: %1   lines: %2|").arg(plainTextEdit->toPlainText().size()).arg(tc.blockNumber() + 1));
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
    searchDialog dia(plainTextEdit);
    dia.exec();
}


void MainWindow::on_action_replace_triggered()
{
    replaceDialog dia(plainTextEdit);
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

    plainTextEdit->clear(); // 发射textChanged信号，isSaved必须后面修改

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
    plainTextEdit->setPlainText(context);
    file.close();

    isSaved = true;
    currFileName = fileName;
    this->setWindowTitle(currFileName);
}


void MainWindow::on_action_save_triggered()
{
    if (isSaved) return;

    if (currFileName != "") {
        QFile file(currFileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "file save failed!");
            return;
        }

        QTextStream tStream(&file);
        tStream.setAutoDetectUnicode(true);
        QString context = plainTextEdit->toPlainText();
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
    QString context = plainTextEdit->toPlainText();
    tStream << context;
    file.flush();
    file.close();

    isSaved = true;
    this->setWindowTitle(currFileName);
    currFileName = fileName;
}

void MainWindow::plainTextEdit_textChanged()
{
    if (isInitialState) {
        isInitialState = false;
        return;
    }
    isSaved = false;
    this->setWindowTitle("*" + (currFileName == "" ? "新建文件" : currFileName));
}



void MainWindow::on_action_toolBar_triggered()
{
    ui->toolBar->setVisible(ui->action_toolBar->isChecked());
}


void MainWindow::on_action_statusbar_triggered()
{
    ui->statusbar->setVisible(ui->action_statusbar->isChecked());
}


void MainWindow::on_action_autoWrap_triggered()
{
    plainTextEdit->setLineWrapMode((QPlainTextEdit::LineWrapMode)ui->action_autoWrap->isChecked());
}


void MainWindow::on_action_fontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "字体颜色");
    if (color.isValid()) {
        QPalette palette = plainTextEdit->palette();
        palette.setColor(QPalette::Text, color);
        plainTextEdit->setPalette(palette);
    }
}


void MainWindow::on_action_backgroundColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "背景颜色");
    if (color.isValid()) {
        QPalette palette = plainTextEdit->palette();
        palette.setColor(QPalette::Base, color);
        plainTextEdit->setPalette(palette);
    }
}


void MainWindow::on_action_fontBackgroundColor_triggered()
{
    // 不支持全局修改，显示行号没有保留格式
    QColor color = QColorDialog::getColor(Qt::white, this, "字体背景颜色");
    if (color.isValid()) {
        QTextCharFormat format = plainTextEdit->currentCharFormat();
        format.setBackground(color);
        plainTextEdit->setCurrentCharFormat(format);
    }
}


void MainWindow::on_action_font_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, plainTextEdit->font(), this);
    if (ok) {
        if (font.pointSize() == 0)
            font.setPointSize(12);
        plainTextEdit->setFont(font);
    }
}


void MainWindow::on_action_showLineNum_triggered()
{
    QPlainTextEdit *t;
    if (ui->action_showLineNum->isChecked())
        t = new CodeEditor();
    else
        t = new QPlainTextEdit();
    t->setPlainText(plainTextEdit->toPlainText());
    t->setFont(plainTextEdit->font());
    t->setPalette(plainTextEdit->palette());
    delete plainTextEdit;
    plainTextEdit = t;
    this->setCentralWidget(t);
    iniSignalSlots();
}

