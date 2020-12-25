//
//  QTNClient.cpp
//  QTNetworkDemo
//
//  Created by mugx on 04/03/12.
//  Copyright (c) 2012-2020 mugx. All rights reserved.

#include "QTNClient.h"
#include <QTNConst.h>
#include <QTimer>
#include <QHostAddress>
#include <QDataStream>

QTNClient::QTNClient()
: SERVER_IP(QTNConst::SERVER_IP),
SERVER_PORT(QTNConst::SERVER_PORT),
m_clientStatus(CLIENT_STATUS_IDLE),
m_blockSize(0) {
    connect(&m_socket, SIGNAL(connected()), this, SLOT(onNewConnection()));
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(onReceiveMessage()));
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onConnectionError()));
}

QTNClient::~QTNClient() {}

bool QTNClient::init() {
    std::cout << getStatusDescription().c_str() << "is connecting to the server " << "ip: " << SERVER_IP << ", port: " << SERVER_PORT << "\n";

    m_socket.connectToHost(SERVER_IP, SERVER_PORT);
    bool isInitialized = m_socket.state() == QAbstractSocket::ConnectingState;

    if (!isInitialized)
    {
        m_clientStatus = CLIENT_STATUS_NOK;
        std::cout << getStatusDescription().c_str() << "is NOT connected to the server\n";
    }

    return isInitialized;
}

void QTNClient::onNewConnection() {
    m_clientStatus = CLIENT_STATUS_OK;
    std::cout << getStatusDescription().c_str() << "is connected to the server\n";
}

void QTNClient::onReceiveMessage() {
    // begin - check the block
    QDataStream inputBlock(&m_socket);
    inputBlock.setVersion(QDataStream::Qt_4_0);

    // first block received
    if (m_blockSize == 0) {
        if (m_socket.bytesAvailable() < static_cast<int>(sizeof(quint16))) {
            return;
        }

        inputBlock >> m_blockSize;
    }

    if (m_socket.bytesAvailable() < m_blockSize) {
        // missing blocks of the message, waiting for the next block
        return;
    }
    // end - check the block

    // got the message...
    QString messageReceived;
    inputBlock >> messageReceived;
    std::string messageReceived_str = messageReceived.toStdString();
    std::cout << getStatusDescription().c_str() << "messageReceived: " << messageReceived_str.c_str() << "\n";

    //... going to parse it
    if (messageReceived_str == QTNConst::MSG_WELCOME) {
        QTimer::singleShot(QTNConst::TIMEOUT_FOR_SENDING_LOGOUT_MSG, this, SLOT(sendLogoutMessage()));
    } else if (messageReceived_str == QTNConst::MSG_GOODBYE) {
        m_socket.disconnectFromHost();
        m_clientStatus = CLIENT_STATUS_DISCONNECTED;
        std::cout << getStatusDescription().c_str() << "\n";
    }

    m_blockSize = 0;
}

void QTNClient::onConnectionError() {
    m_clientStatus = CLIENT_STATUS_NOK;
    std::cout << getStatusDescription().c_str() << "connection error\n";
}

void QTNClient::sendLogoutMessage() {
    // preparing welcome message
    QByteArray block;
    QDataStream out(&block, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_0);

    out << (quint16)0;
    out << QString(QTNConst::MSG_LOGOUT);
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    out.device()->seek(0);

    // sending welcome message
    std::cout << getStatusDescription().c_str() << "is sending logout message to " << m_socket.peerAddress().toString().toStdString() << "\n";
    m_socket.write(block);
}

std::string QTNClient::getStatusDescription() {
    std::stringstream descr;
    descr << "[QTNDemo Client,";

    switch (m_clientStatus) {
    case CLIENT_STATUS_IDLE:
        descr << "IDLE] ";
        break;
    case CLIENT_STATUS_OK:
        descr << "OK] ";
        break;
    case CLIENT_STATUS_NOK:
        descr << "NOK] ";
        break;
    case CLIENT_STATUS_DISCONNECTED:
        descr << "DISCONNECTED] ";
        break;
    }

    return descr.str();
}
