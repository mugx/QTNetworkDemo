//
//  QTNServer.cpp
//  QTNetworkDemo
//
//  Created by mugx on 04/03/12.
//  Copyright (c) 2012-2020 mugx. All rights reserved.

#include "QTNServer.h"
#include "QTNServerThread.h"
#include <QTNConst.h>
#include <QTcpSocket>
#include <QStringList>
#include <QTimer>
#include <QThreadPool>

QTNServer::QTNServer()
: SERVER_IP(QTNConst::SERVER_IP),
SERVER_PORT(QTNConst::SERVER_PORT),
m_serverStatus(SERVER_STATUS_IDLE),
m_numMaxClients(QTNConst::NUM_MAX_CLIENTS)
{}

bool QTNServer::init() {
    std::cout << getStatusDescription().c_str() << "is initializing " << "ip: " << SERVER_IP << ", port: " << SERVER_PORT << "\n";

    bool isInitialized = listen(QHostAddress(SERVER_IP), SERVER_PORT);
    if (isInitialized) {
        m_serverStatus = SERVER_STATUS_OK;
        std::cout << getStatusDescription().c_str() << "is listening\n";
        QThreadPool::globalInstance()->setMaxThreadCount(QTNConst::NUM_MAX_CLIENTS);
        connect(this, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
        QTimer::singleShot(1000, this, SLOT(printClients()));
    } else {
        m_serverStatus = SERVER_STATUS_NOK;
        std::cout << getStatusDescription().c_str() << "is NOT listening\n";
    }

    return isInitialized;
}

void QTNServer::onNewConnection() {
    // sending welcome message to the client
    QTcpSocket* clientConnection = nextPendingConnection();
    if (clientConnection) {
        // check if we are reaching num max of clients connected
        if (m_clients.size() < m_numMaxClients) {
            // very lightweight task ready to use, will not allocate OS Thread
            QTNServerThread* serverTask = new QTNServerThread(clientConnection->socketDescriptor(), this);
            m_clients[clientConnection->socketDescriptor()] = serverTask;
            QThreadPool::globalInstance()->start(serverTask);
        } else {
            clientConnection->disconnectFromHost();
        }
    }
}

void QTNServer::printClients() {
    std::cout << getStatusDescription().c_str() << "clients connected: ";
    int i = 0;
    for (QMap<int, QTNServerThread*>::iterator it = m_clients.begin(), end = m_clients.end();it != end;++it) {
        std::cout << "\n	[client num " << ++i << "/" << m_numMaxClients << "] " << (*it)->getClientConnectedIp() << ";";
    }

    if (i == 0) {
        std::cout << "NONE";
    }

    std::cout << "\n";

    QTimer::singleShot(1000, this, SLOT(printClients()));
}

std::string QTNServer::getStatusDescription() {
    std::stringstream descr;
    descr << "[QTNDemo Server,";

    switch (m_serverStatus) {
    case SERVER_STATUS_IDLE:
        descr << "IDLE] ";
        break;
    case SERVER_STATUS_OK:
        descr << "OK] ";
        break;
    case SERVER_STATUS_NOK:
        descr << "NOK] ";
        break;
    }

    return descr.str();
}
