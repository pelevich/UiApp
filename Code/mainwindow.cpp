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

void MainWindow::appendMessage(QTextBrowser *ChatDisplay,const QString &sender, const QString &message, bool isOutgoing)
{
    QString side = isOutgoing ? "right" : "left";
    QString messageClass = isOutgoing ? "outgoing" : "incoming";
    QString color = isOutgoing ? "#DCF8C6" : "#E8E8E8";

    QString html = QString(
            "<table width='100%'>"
            "<tr><td align='%1'>"
            "<div class='message %2' style='background-color: %3; display: inline-block; padding: 8px; border-radius: 10px; margin: 5px; max-width: 70%;'>"
            "<span class='sender'>%4</span><br>"
            "%5<br>"
            "</div>"
            "</td></tr>"
            "</table>"
        ).arg(side).arg(messageClass).arg(color).arg(sender).arg(message);

    ChatDisplay->moveCursor(QTextCursor::End);
    ChatDisplay->insertHtml(html);
}

void MainWindow::on_pushButton_clicked()
{
    if (A) {
        A->hPipe = A->ConnectedServer("MyPipe");
        A->Write(A->hPipe, A->send_message);
        appendMessage(ui->chatDisplay, QString::fromStdString("Client"),QString::fromStdString(A->send_message), true);
        A->Read(A->hPipe, A->servBuff, sizeof(A->servBuff));
        A->servBuff;
        QString res = QString::fromStdString(A->servBuff);
        appendMessage(ui->chatDisplay, QString::fromStdString("Service"),res, false);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    if(A){

        ui->chatDisplay_2->clear();
        int currentValue = ui->spinBox->value();
        string send_message = "RandomLoad " + to_string(currentValue);

        A->hPipeCallFunction = A->ConnectedServer("MyPipeCallFunction");
        A->Write(A->hPipeCallFunction, send_message);

        for(int i = 0; i < currentValue; i++){
            A->Read(A->hPipeCallFunction, A->servBuff, sizeof(A->servBuff));
            appendMessage(ui->chatDisplay_2, QString::fromStdString("Service"),QString::fromStdString(A->servBuff), false);
            QApplication::processEvents();
        }
    }
}

