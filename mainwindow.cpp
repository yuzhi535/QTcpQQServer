#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QMap>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //ui设计
    ui->setupUi(this);
    view = new QGraphicsView(this);
    view->setStatusTip(QString("显示图片"));
    button_1.setText(QString("监听"));
    button_2.setText(QString("退出"));
    //set shortcut
    button_1.setShortcut(QString("return"));
    button_2.setShortcut(QString("esc"));
    label_1.setText(QString("ip"));
    label_2.setText(QString("port"));

    menu = menuBar()->addMenu(tr("关于"));
    about = new QAction("作者", this);
    menu->addAction(about);

    connect(about, &QAction::triggered, [&]() {
        QMessageBox::information(this, QString("通知"),
                                 QString("<h1>作者：周誉喜</h1><h2>代码已放在github上</h2><h3>"
                                         "地址: https://github.com/yuzhi535/QTcpQQServer</h3>"));
    });

    connect(&text, &QListWidget::doubleClicked, [&] () {
        QString msg = text.currentItem()->text();
        QMessageBox::information(this, QString("通知"), QString("<h3>" + msg + "</h3>"));
    });

    this->setCentralWidget(&contral);
    contral.setLayout(&m_layout);
    //length 20 width 16
    m_layout.setHorizontalSpacing(16);
    m_layout.setVerticalSpacing(20);

    line_1.resize(4, 8);
    line_2.resize(4, 8);
    line_1.setStatusTip(QString("最好不要动"));
    line_2.setStatusTip(QString("最好不要动"));

    label_1.setStatusTip(tr("ip"));
    label_1.setAlignment(Qt::AlignCenter);   //居中
    label_2.setStatusTip(tr("port"));
    label_2.setAlignment(Qt::AlignCenter);
    text.setStatusTip(QString("显示信息"));

    m_layout.addWidget(&button_1, 1, 14, 1, 2);    //connect
    m_layout.addWidget(&button_2, 2, 14, 1, 2);    //exit
    m_layout.addWidget(&label_1, 1, 6, 1, 1);
    m_layout.addWidget(&label_2, 2, 6, 1, 1);
    m_layout.addWidget(&line_1, 1, 7, 1, 7);
    m_layout.addWidget(&line_2, 2, 7, 1, 7);
    m_layout.addWidget(view, 6, 12, 10, 8);
    m_layout.addWidget(&text, 6, 0, 10, 12);

    line_1.setText(QString("127.0.0.1"));
    line_2.setText(QString("18080"));

    connect(&button_1, &QPushButton::clicked, this, &MainWindow::on_button_1_clicked);
    connect(&button_2, &QPushButton::clicked, [&](){
        if (server->isListening())     //if is listening, disconnect
            server->close();
        this->close();    //关闭窗口
        exit(0);          //应用结束
    });
    server = new MyServer();

    connect(server, SIGNAL(newUser(QString)), this, SLOT(newUser(QString)));         //新用户
    connect(server, SIGNAL(new_Msg(QString)), this, SLOT(newMsg(QString)));          //新消息
    connect(server, SIGNAL(oldUser(QString)), this, SLOT(old_user(QString)));        //断开连接
    connect(server, SIGNAL(showImg(QByteArray)), this, SLOT(show_img(QByteArray)));  //显示图片


//    用户信息处理
#if 0
    QMap<QString, QString> login;
    QFile loginFile(QString("login.json"));
    loginFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (loginFile.isOpen())
    {
        QString value = loginFile.readAll();
        loginFile.close();
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(value.toUtf8(), &err);
        if (err.error != QJsonParseError::NoError)     //若解析错误
        {
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
                login[val["name"].toString()] = val["password"].toString();    //登记用户信息
            }
        }
    }
    else    // 其实应该不会发生
    {
        QMessageBox::information(this, QString("错误"), QString("无法获取用户信息，即将关闭!\n"));
        this->close();
        exit(0);
    }
#endif
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
            QMessageBox::warning(this, QString("警告"), QString("监听失败"));
    }
    else
    {
        QMessageBox::warning(this, QString("错误"), QString("您已开始监听!"));
    }
}

void MainWindow::newUser(QString name)
{
     name.remove(QRegularExpression("\b.*\b\r"));
    text.addItem(name);
}

void MainWindow::newMsg(QString msg)
{
    text.addItem(msg);
}

void MainWindow::old_user(QString user)
{
    text.addItem(user);
}

void MainWindow::show_img(QByteArray img)
{
    if (!img.isEmpty())
    {
        QMutex mutex;
        mutex.lock();
        QPixmap tmp;
        tmp.loadFromData(img, "PNG");
        QBuffer buffer;
        buffer.open(QIODevice::ReadWrite);
        tmp.save(&buffer, "PNG");                 //在buffer中存储这个图片，后缀是.png

        QByteArray dataArray;
        dataArray.append(buffer.data());

        QGraphicsScene* scene = new QGraphicsScene;
        scene->addPixmap(tmp);
        view->setScene(scene);
        view->show();
        mutex.unlock();
    }
    else
    {
        QMessageBox::critical(this, "通知", "<h1>图片未传送到</h1>");
    }
}
