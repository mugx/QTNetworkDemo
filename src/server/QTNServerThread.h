//
//  QTNServerThread.h
//  QTNetworkDemo
//
//  Created by mugx on 04/03/12.
//  Copyright (c) 2012-2020 mugx. All rights reserved.

#ifndef QTNServerThread_h
#define QTNServerThread_h

#include "QTNServer.h"
#include <QRunnable>
#include <QTcpSocket>

class QTNServerThread : public QObject, public QRunnable {
    Q_OBJECT
public:
    friend class QTNServer;

    enum ServerThreadStatus {
        SERVER_THREAD_STATUS_IDLE = 0,
        SERVER_THREAD_STATUS_OK,
        SERVER_THREAD_STATUS_NOK,
        SERVER_THREAD_STATUS_DISCONNECTED
    };

    QTNServerThread(int socketDescriptor, QTNServer* _server);
    ~QTNServerThread();
    std::string getClientConnectedIp();
signals:
    void error(int socketError, const QString &message);

protected:
    void run();

private:
    std::string getStatusDescription();
    void initSocket();
    void sendWelcomeMessage();
    void receiveLogoutMessage();
    void sendGoodbyeMessage();

    std::string m_ipAddress;
    QTNServer* m_server;
    ServerThreadStatus m_serverThreadStatus;
    QTcpSocket* m_socket;
    int m_socketDescriptor;
};

#endif
