#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Client *Object, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    void appendMessage(QTextBrowser *ChatDisplay, const QString &sender, const QString &message, bool isOutgoing = false);

private:
    Ui::MainWindow *ui;
    Client *A;
};
#endif // MAINWINDOW_H
