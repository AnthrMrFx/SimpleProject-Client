#include "client.h"

Client::Client(const QString& strHost,
               int nPort,
               QWidget *parent)
    : QWidget(parent),
      tcpSocket(new QTcpSocket(this)),
      txtInfo(new QTextEdit),
      txtInput(new QLineEdit),
      serverEdit(new QLineEdit),
      serverLabel(new QLabel),
      portEdit(new QLineEdit),
      portLabel(new QLabel)
{
//    tcpSocket = new QTcpSocket(this);
//    tcpSocket->connectToHost(strHost, nPort);

//    connect(tcpSocket, &QTcpSocket::connected,
//            this, &Client::slotConnected);
//    connect(tcpSocket, &QTcpSocket::readyRead,
//            this, &Client::slotReadyRead);
//    connect(tcpSocket, &QTcpSocket::errorOccurred,
//            this, &Client::slotError);

    txtInfo->setReadOnly(true);

    sendMsgButton = new QPushButton(tr("Отправить"));
//    sendMsgButton->setEnabled(false);
    connect(sendMsgButton, &QPushButton::clicked,
            this, &Client::slotSendToServer);
//    connect(sendMsgButton, &QLineEdit::textChanged,
//            this, &Client::slotEnableSendMsgButton);

    connectButton = new QPushButton(tr("Подключиться"));
//    connectButton->setEnabled(false);
    connect(connectButton, &QPushButton::clicked,
            this, &Client::slotConnectToServer);
//    connect(portEdit, &QLineEdit::textChanged,
//            this, &Client::slotEnableConnectButton);

    disconnectButton = new QPushButton(tr("Отключиться"));
//    disconnectButton->setEnabled(false);
    connect(disconnectButton, &QPushButton::clicked,
            this, &Client::slotDisconnectFromServer);
//    connect(disconnectButton, &QLineEdit::textChanged,
//            this, &Client::slotEnableDisconnectButton);

    connect(txtInput, &QLineEdit::returnPressed,
            this, &Client::slotSendToServer);

//    QHBoxLayout* serverHBoxLayout = new QHBoxLayout;
//    QHBoxLayout* portHBoxLayout = new QHBoxLayout;

    serverLabel->setText(tr("Сервер: "));
    portLabel->setText(tr("Порт: "));

    QVBoxLayout* vBoxLayout = new QVBoxLayout;
    QGridLayout* grid = new QGridLayout;

    grid->addWidget(serverLabel, 0, 0);
    grid->addWidget(serverEdit, 0, 1);
    grid->addWidget(portLabel, 1, 0);
    grid->addWidget(portEdit, 1, 1);

//    serverHBoxLayout->addWidget(serverLabel);
//    serverHBoxLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
//    serverHBoxLayout->addWidget(serverEdit);
//    portHBoxLayout->addWidget(portLabel);
//    portHBoxLayout->addWidget(portEdit);

    vBoxLayout->addWidget(new QLabel(tr("<H1>Клиент</H1>")));
    vBoxLayout->addItem(grid);
//    vBoxLayout->addItem(serverHBoxLayout);
//    vBoxLayout->addItem(portHBoxLayout);
    vBoxLayout->addWidget(txtInfo);
    vBoxLayout->addWidget(txtInput);
    vBoxLayout->addWidget(connectButton);
    vBoxLayout->addWidget(disconnectButton);
    vBoxLayout->addWidget(sendMsgButton);

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

//        QDataStream in(tcpSocket);
//        in.setVersion(QDataStream::Qt_5_3);
//        for (;;)
//        {
//            if (!m_nNextBlockSize)
//            {
//                qDebug() << "case1: " << m_nNextBlockSize << " | " << m_nNextBlockSize;;
//                if (tcpSocket->bytesAvailable() < sizeof(quint16)) {
//                    qDebug() << "case2: " << tcpSocket->bytesAvailable() << " | " << m_nNextBlockSize;;
//                    break;
//                }
//                in >> m_nNextBlockSize;
//            }
//            if (tcpSocket->bytesAvailable() < m_nNextBlockSize)
//            {
//                qDebug() << "case3" << tcpSocket->bytesAvailable() << " | " << m_nNextBlockSize;
//                break;
//            }
//        QTime time;
//        QString str;
//        in >> time >> str;
//        txtInfo->append(time.toString() +" "+ str);
//        m_nNextBlockSize = 0;
//    }
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
//    QByteArray block;
//    QDataStream out(&block, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_10);

//    out << fortunes[QRandomGenerator::global()->bounded(int(fortunes.size()))];
////! [4] //! [7]

//    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
//    connect(clientConnection, &QAbstractSocket::disconnected,
//            clientConnection, &QObject::deleteLater);
////! [7] //! [8]

//    clientConnection->write(block);

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out /*<< quint16(0)*/ << QTime::currentTime() << txtInput->text();
//    out.device()->seek(0);             Убирается чтобы сервер мог принимать сообщение
//    out << quint16(arrBlock.size() - sizeof(quint16));
    tcpSocket->write(arrBlock);
    qDebug() << arrBlock;
    txtInput->setText("");
}

void Client::slotConnected()
{
    qDebug() << "point 2";
    txtInfo->append(tr("Соединение установлено"));
}

Client::~Client()
{
}

void Client::slotConnectToServer()
{
    tcpSocket->connectToHost(serverEdit->text(), portEdit->text().toInt());

    qDebug() << "point 1";

    connect(tcpSocket, &QTcpSocket::connected,
            this, &Client::slotConnected);
    connect(tcpSocket, &QTcpSocket::readyRead,
            this, &Client::slotReadyRead);
    connect(tcpSocket, &QTcpSocket::errorOccurred,
            this, &Client::slotError);
}

//void Client::slotEnableSendMsgButton()
//{
//    sendMsgButton->setEnabled((!portEdit->text().isEmpty())&&(!serverEdit->text().isEmpty()));
//}

//void Client::slotEnableConnectButton()
//{
//    connectButton->setEnabled((!portEdit->text().isEmpty())&&(!serverEdit->text().isEmpty()));
//}

//void Client::slotEnableDisconnectButton()
//{
//    disconnectButton->setEnabled((!portEdit->text().isEmpty())&&(!serverEdit->text().isEmpty()));
//}

void Client::slotDisconnectFromServer()
{
    txtInfo->append(tr("Соединение разорвано"));
    tcpSocket->disconnectFromHost();
}

