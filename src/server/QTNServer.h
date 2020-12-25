//
//  QTNServer.h
//  QTNetworkDemo
//
//  Created by mugx on 04/03/12.
//  Copyright (c) 2012-2020 mugx. All rights reserved.

#ifndef QTNServer_H
#define QTNServer_H

#include <QTcpServer>
#include <iostream>
#include <string>
#include <sstream>

class QTNServerThread;

class QTNServer : public QTcpServer
{
    Q_OBJECT

public:
    friend class QTNServerThread;

    enum ServerStatus
    {
        SERVER_STATUS_IDLE = 0,
        SERVER_STATUS_OK = 1,
        SERVER_STATUS_NOK = 2
    };

    QTNServer();
    bool init();

public slots:
    void onNewConnection();
    void printClients();

private:
    std::string getStatusDescription();

    const char* SERVER_IP;
    int SERVER_PORT;
    ServerStatus m_serverStatus;
    QMap<int, QTNServerThread*> m_clients;
    int m_numMaxClients;
};

#endif
