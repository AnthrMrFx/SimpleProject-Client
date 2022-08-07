#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QDialog>
#include <QtWidgets>
#include <QString>
#include <QTextEdit>
#include <QtNetwork>

class Client : public QWidget
{
    Q_OBJECT

private:
    QTcpSocket* tcpSocket;

    QTextEdit* txtInfo;
    QLineEdit* txtInput;
    QLineEdit* serverEdit;
    QLabel* serverLabel;
    QLineEdit* portEdit;
    QLabel* portLabel;
    QPushButton* connectButton;
    QPushButton* sendMsgButton;
    QPushButton* disconnectButton;

//    quint16 m_nNextBlockSize;
    QDataStream in;
public:
    Client(const QString& strHost, int nPort, QWidget *parent = nullptr);
    ~Client();

private slots:
    void slotConnectToServer();
//    void slotEnableSendMsgButton();
//    void slotEnableConnectButton();
//    void slotEnableDisconnectButton();
    void slotDisconnectFromServer();
    void slotReadyRead () ;
    void slotError(QAbstractSocket::SocketError) ;
    void slotSendToServer( );
    void slotConnected () ;
};
#endif // CLIENT_H
