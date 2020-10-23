#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QMap>
//#include <iostream>

//#include <

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    button_1.setText(QString("监听"));
    button_2.setText(QString("退出"));
    //set shortcut
    button_1.setShortcut(QString("return"));
    button_2.setShortcut(QString("esc"));
    label_1.setText(QString("ip"));
    label_2.setText(QString("port"));

    this->setCentralWidget(&contral);
    contral.setLayout(&m_layout);
    //length 20 width 16
    m_layout.setHorizontalSpacing(16);
    m_layout.setVerticalSpacing(20);

    line_1.resize(4, 8);
    line_2.resize(4, 8);
    line_1.setStatusTip(tr("最好不要动"));
    line_2.setStatusTip(tr("最好不要动"));

    label_1.setStatusTip(tr("ip"));
    label_1.setAlignment(Qt::AlignCenter);
    label_2.setStatusTip(tr("port"));
    label_2.setAlignment(Qt::AlignCenter);
    text.setStatusTip(QString("the info listened"));
    sysLog.setStatusTip(QString("the info system generate"));

    m_layout.addWidget(&button_1, 1, 14, 1, 2);    //connect
    m_layout.addWidget(&button_2, 2, 14, 1, 2);    //exit
    m_layout.addWidget(&label_1, 1, 6, 1, 1);
    m_layout.addWidget(&label_2, 2, 6, 1, 1);
    m_layout.addWidget(&line_1, 1, 7, 1, 7);
    m_layout.addWidget(&line_2, 2, 7, 1, 7);
    m_layout.addWidget(&sysLog, 6, 12, 10, 8);
    m_layout.addWidget(&text, 6, 0, 10, 12);

    line_1.setText(QString("127.0.0.1"));
    line_2.setText(QString("8080"));

//    qDebug() << m_layout.rowCount() << m_layout.columnCount();

//    qDebug() << label_1.size().width() << label_1.size().height();

    connect(&button_1, &QPushButton::clicked, this, &MainWindow::on_button_1_clicked);
    connect(&button_2, &QPushButton::clicked, [&](){
        if (server->isListening())     //if is listening, disconnect
            server->close();
        this->close();    //关闭窗口
        exit(0);          //应用结束
    });
    server = new MyServer();


    QMap<QString, QString> login;
    QFile loginFile(QString("login.json"));
    loginFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (loginFile.isOpen())
    {
        QString value = loginFile.readAll();
        loginFile.close();
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(value.toUtf8(), &err);
        if (err.error != QJsonParseError::NoError)
        {
//            std::cout << QDir::currentPath().toStdString() << std::endl;
//            qDebug() << "can't open the json file!";
            printf("can't open the json file!\n");
            close();
            exit(0);
        }
        else
        {
            QJsonArray arr = doc.array();
            for (int i = 0; i < arr.size(); ++i)
            {
                QJsonValue val = arr.at(i);
                qDebug() << val["name"];
                qDebug() << val["password"];
            }
        }
    }
    else    // 其实应该不会发生
    {
        QMessageBox::information(this, QString("错误"), QString("无法获取用户信息，即将关闭!\n"));
        this->close();
        exit(0);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
}

void MainWindow::on_button_1_clicked()
{
    qDebug() << "seccess";
    if (!server->isListening())
    {
        bool flag = server->listen(QHostAddress(line_1.text()), line_2.text().toUInt());
        if (!flag)
            QMessageBox::warning(this, tr("警告"), tr("监听失败"));
        /*
        a new client info
        connect failed
        the info client send
        show the info client send
        */
        //        connect(server,)
        else
        {

        }
    }
    else
    {
        QMessageBox::warning(this, QString("错误"), QString("您已开始监听!"));
    }
}
