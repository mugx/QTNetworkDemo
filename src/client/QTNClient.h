//
//  QTNClient.h
//  QTNetworkDemo
//
//  Created by mugx on 04/03/12.
//  Copyright (c) 2012-2020 mugx. All rights reserved.

#ifndef QTNClient_h
#define QTNClient_h

#include <QObject>
#include <QTcpSocket>
#include <iostream>
#include <string>
#include <sstream>

class QTNClient : QObject {
    Q_OBJECT
public:
    enum ClientStatus {
        CLIENT_STATUS_IDLE = 0,
        CLIENT_STATUS_OK,
        CLIENT_STATUS_NOK,
        CLIENT_STATUS_DISCONNECTED
    };

    QTNClient();
    ~QTNClient();
    bool init();

public slots:
    void onNewConnection();
    void onReceiveMessage();
    void onConnectionError();
    void sendLogoutMessage();

private:
    std::string getStatusDescription();

    const char* SERVER_IP;
    int SERVER_PORT;
    QTcpSocket m_socket;
    ClientStatus m_clientStatus;
    quint16 m_blockSize;
};

#endif
