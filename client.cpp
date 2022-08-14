#include "client.h"

Client::Client(QWidget *parent)
    : QWidget(parent),
      tcpSocket(new QTcpSocket(this)),
      txtInfo(new QTextEdit),
      txtInput(new QLineEdit),
      serverEdit(new QLineEdit),
      serverLabel(new QLabel),
      portEdit(new QLineEdit),
      portLabel(new QLabel)
{

    txtInfo->setReadOnly(true);

    sendMsgButton = new QPushButton(tr("Отправить"));
    connect(sendMsgButton, &QPushButton::clicked,
            this, &Client::slotSendToServer);

    connectButton = new QPushButton(tr("Подключиться"));
    connect(connectButton, &QPushButton::clicked,
            this, &Client::slotConnectToServer);

    disconnectButton = new QPushButton(tr("Отключиться"));
    connect(disconnectButton, &QPushButton::clicked,
            this, &Client::slotDisconnectFromServer);


    connect(txtInput, &QLineEdit::returnPressed,
            this, &Client::slotSendToServer);
    connect(tcpSocket, &QTcpSocket::connected,
            this, &Client::slotConnected);
    connect(tcpSocket, &QTcpSocket::readyRead,
            this, &Client::slotReadyRead);
    connect(tcpSocket, &QTcpSocket::errorOccurred,
            this, &Client::slotError);

    serverLabel->setText(tr("Сервер: "));
    portLabel->setText(tr("Порт: "));

    QVBoxLayout* vBoxLayout = new QVBoxLayout;
    QGridLayout* grid = new QGridLayout;

    grid->addWidget(serverLabel, 0, 0);
    grid->addWidget(serverEdit, 0, 1);
    grid->addWidget(portLabel, 1, 0);
    grid->addWidget(portEdit, 1, 1);

    vBoxLayout->addWidget(new QLabel(tr("<H1>Клиент</H1>")));
    vBoxLayout->addItem(grid);
    vBoxLayout->addWidget(txtInfo);
    vBoxLayout->addWidget(txtInput);
    vBoxLayout->addWidget(sendMsgButton);
    vBoxLayout->addWidget(connectButton);
    vBoxLayout->addWidget(disconnectButton);

    setLayout(vBoxLayout);
}

void Client::slotReadyRead ()
{
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_5_10);

    in.startTransaction();

    QTime time;
    QString msg;
    in >> time >> msg;

    if (!in.commitTransaction())
    {
        return;
    }

    txtInfo->append(time.toString() +" "+ msg);
}

void Client::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            tr("Ошибка ") + (err == QAbstractSocket::HostNotFoundError?
                        tr("Хост не найден.") :
                        err == QAbstractSocket::RemoteHostClosedError?
                        tr("Хост закрыт.") :
                        err == QAbstractSocket::ConnectionRefusedError?
                        tr("Соединение отклонено") :
                        QString(tcpSocket->errorString())
                        );

    txtInfo->append(strError);

}

void Client::slotSendToServer()
{
    if( txtInput->text().isEmpty())
    {
        return;
    }

    QByteArray arrBlock;

    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out << QTime::currentTime() << txtInput->text();

    tcpSocket->write(arrBlock);
    txtInput->setText("");
}

void Client::slotConnected()
{
    txtInfo->append(tr("Соединение установлено"));
}

void Client::slotConnectToServer()
{
    tcpSocket->connectToHost(serverEdit->text(), portEdit->text().toInt());
}

void Client::slotDisconnectFromServer()
{
    txtInfo->append(tr("Соединение разорвано"));
    tcpSocket->disconnectFromHost();
}

