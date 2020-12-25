//
//  QTNServerThread.cpp
//  QTNetworkDemo
//
//  Created by mugx on 04/03/12.
//  Copyright (c) 2012-2020 mugx. All rights reserved.

#include "QTNServerThread.h"
#include <QTNConst.h>
#include <QDataStream>

QTNServerThread::QTNServerThread(int socketDescriptor, QTNServer* _server)
: m_server(_server),
m_serverThreadStatus(SERVER_THREAD_STATUS_IDLE),
m_socket(0),
m_socketDescriptor(socketDescriptor) {
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

QTNServerThread::~QTNServerThread() {
    m_server->m_clients.remove(m_socketDescriptor); // release the thread from thread map
    m_ipAddress.clear();

    if (m_socket) {
        delete m_socket;
        m_socket = 0;
    }
}

void QTNServerThread::run() {
    initSocket();
    sendWelcomeMessage();
    receiveLogoutMessage();
}

void QTNServerThread::initSocket() {
    m_socket = new QTcpSocket();
    if (!m_socket->setSocketDescriptor(m_socketDescriptor)) {
        m_serverThreadStatus = SERVER_THREAD_STATUS_NOK;
        std::cout << getStatusDescription().c_str() << "invalid socket descriptor\n";
        return;
    } else {
        m_serverThreadStatus = SERVER_THREAD_STATUS_OK;
        std::cout << getStatusDescription().c_str() << "socket initialized\n";
        m_ipAddress = m_socket->peerAddress().toString().toStdString();
    }
}

void QTNServerThread::sendWelcomeMessage() {
    if (m_socket == 0 || !m_socket->isValid()) return;

    // preparing welcome message
    QByteArray block;
    QDataStream out(&block, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_0);

    out << (quint16)0;
    out << QString(QTNConst::MSG_WELCOME);
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    out.device()->seek(0);

    // sending welcome message
    std::cout << getStatusDescription().c_str() << "is sending welcome message to " << m_socket->peerAddress().toString().toStdString() << "\n";
    m_socket->write(block);
}

void QTNServerThread::receiveLogoutMessage() {
    if (m_socket == 0 || !m_socket->isValid()) return;

    //--- waiting for the logout ---//
    // waiting for the size of packet
    while (m_socket->bytesAvailable() < static_cast<int>(sizeof(quint16))) {
        if (!m_socket->waitForReadyRead(QTNConst::SOCKET_TIMEOUT)) {
            m_serverThreadStatus = SERVER_THREAD_STATUS_NOK;
            std::cout << getStatusDescription().c_str() << "socket timeout\n";
            return;
        } else {
            continue;
        }
    }

    quint16 blockSize;
    QDataStream inputBlock(m_socket);
    inputBlock.setVersion(QDataStream::Qt_4_0);
    inputBlock >> blockSize;

    std::cout << getStatusDescription().c_str() << "m_blockSize: " << blockSize << "\n";
    while (m_socket->bytesAvailable() < blockSize) {
        if (!m_socket->waitForReadyRead(QTNConst::SOCKET_TIMEOUT)) {
            m_serverThreadStatus = SERVER_THREAD_STATUS_NOK;
            std::cout << getStatusDescription().c_str() << "socket timeout\n";
            return;
        } else {
            continue;
        }
    }

    QString messageReceived;
    inputBlock >> messageReceived;
    std::string messageReceived_str = messageReceived.toStdString();
    std::cout << getStatusDescription().c_str() << "messageReceived: " << messageReceived_str.c_str() << "\n";

    //... going to parse it
    if (messageReceived_str == QTNConst::MSG_LOGOUT) {
        sendGoodbyeMessage();
    }

    blockSize = 0;
}

void QTNServerThread::sendGoodbyeMessage() {
    if (m_socket == 0 || !m_socket->isValid()) return;

    //--- SENDING GOODBYE ---//
    // preparing goodbye message
    QByteArray block;
    QDataStream out(&block, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_0);

    out << (quint16)0;
    out << QString(QTNConst::MSG_GOODBYE);
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    out.device()->seek(0);

    // sending welcome message
    std::cout << getStatusDescription().c_str() << "is sending goodbye message to " << m_socket->peerAddress().toString().toStdString() << "\n";
    m_socket->write(block);

    //--- disconnection ---//
    std::cout << getStatusDescription().c_str() << "waiting for disconnection from the client\n";
    m_socket->waitForDisconnected(QTNConst::SOCKET_TIMEOUT);
    m_serverThreadStatus = SERVER_THREAD_STATUS_DISCONNECTED;
    std::cout << getStatusDescription().c_str() << "\n";
    m_server->m_clients.remove(m_socketDescriptor); // release the thread from thread map
}

std::string QTNServerThread::getClientConnectedIp() {
    return m_ipAddress;
}

std::string QTNServerThread::getStatusDescription() {
    std::stringstream descr;
    descr << "[QTNDemo Server Thread," << m_socketDescriptor << ",";

    switch (m_serverThreadStatus)
    {
    case SERVER_THREAD_STATUS_IDLE:
        descr << "IDLE] ";
        break;
    case SERVER_THREAD_STATUS_OK:
        descr << "OK] ";
        break;
    case SERVER_THREAD_STATUS_NOK:
        descr << "NOK] ";
        break;
    case SERVER_THREAD_STATUS_DISCONNECTED:
        descr << "DISCONNECTED] ";
        break;
    }

    return descr.str();
}
