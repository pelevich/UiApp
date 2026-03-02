#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(Client *Object, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , A(Object)
{
    ui->setupUi(this);
    ui->chatDisplay->setReadOnly(true);
    ui->chatDisplay->setOpenExternalLinks(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendMessage(const QString &sender, const QString &message, bool isOutgoing)
{
    QString html;

    if (isOutgoing) {
        // Исходящее сообщение (справа)
        html = QString(
            "<table width='100%'>"
            "<tr><td align='right'>"
            "<div class='message outgoing' style='background-color: #DCF8C6; display: inline-block; padding: 8px; border-radius: 10px; margin: 5px; max-width: 70%;'>"
            "<span class='sender'>%1</span><br>"
            "%2<br>"
            "</div>"
            "</td></tr>"
            "</table>"
        ).arg(sender).arg(message);
    } else {
        // Входящее сообщение (слева)
        html = QString(
            "<table width='100%'>"
            "<tr><td align='left'>"
            "<div class='message incoming' style='background-color: #E8E8E8; display: inline-block; padding: 8px; border-radius: 10px; margin: 5px; max-width: 70%;'>"
            "<span class='sender'>%1</span><br>"
            "%2<br>"
            "</div>"
            "</td></tr>"
            "</table>"
        ).arg(sender).arg(message);
    }

    ui->chatDisplay->moveCursor(QTextCursor::End);
    ui->chatDisplay->insertHtml(html);
}

void MainWindow::on_pushButton_clicked()
{
    if (A) {
        A->Write(A->hPipe, A->send_message);
        appendMessage(QString::fromStdString("Client"),QString::fromStdString(A->send_message), true);
        A->Read(A->hPipe, A->servBuff, sizeof(A->servBuff));
        QString res = QString::fromStdString(A->servBuff);
        appendMessage(QString::fromStdString("Service"),res, false);
    }
}

