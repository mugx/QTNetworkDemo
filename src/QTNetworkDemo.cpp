//
//  QTNetworkDemo.cpp
//  QTNetworkDemo
//
//  Created by mugx on 04/03/12.
//  Copyright (c) 2012-2020 mugx. All rights reserved.

#include "QTNetworkDemo.h"
#include "client/QTNClient.h"
#include "server/QTNServer.h"
#include <QTNConst.h>

QTNetworkDemo::QTNetworkDemo(int argc, char *argv[])
: QCoreApplication(argc, argv),
CLIENT_CHOICE_STR(QTNConst::CLIENT_CHOICE_STR),
SERVER_CHOICE_STR(QTNConst::SERVER_CHOICE_STR),
EXIT_CHOICE_STR(QTNConst::EXIT_CHOICE_STR),
m_opCode(OP_CODE_NOP)
{}

void QTNetworkDemo::printWelcome() {
    std::cout << "--- Welcome to the QTNDemo! ---\n";
    std::cout << "Choose:\n";
    std::cout << "(" << CLIENT_CHOICE_STR << ") Client\n";
    std::cout << "(" << SERVER_CHOICE_STR << ") Server\n";
    std::cout << "(" << EXIT_CHOICE_STR << ") exit from QTNDemo\n";
}

void QTNetworkDemo::askOperation() {
    std::stringstream result;
    std::string choosen;

    do {
        std::cin >> choosen;
        if (choosen == CLIENT_CHOICE_STR) {
            result << "Client selected...";
            m_opCode = OP_CODE_CLIENT_SELECTED;
        } else if (choosen == SERVER_CHOICE_STR) {
            result << "Server selected...";
            m_opCode = OP_CODE_SERVER_SELECTED;
        } else if (choosen == EXIT_CHOICE_STR) {
            result << "Goodbye...";
            m_opCode = OP_CODE_EXIT;
        } else {
            result << "Wrong code!";
            m_opCode = OP_CODE_ERROR;
        }

        std::cout << result.str().c_str() << "\n\n";
        result.str("");
    } while(m_opCode == OP_CODE_ERROR);
}

int QTNetworkDemo::exec() {
    bool isInitialized;

    QTNClient client;
    QTNServer server;

    if (m_opCode == OP_CODE_CLIENT_SELECTED) {
        isInitialized = client.init();

    } else if (m_opCode == OP_CODE_SERVER_SELECTED) {
        isInitialized = server.init();
    }

    if (m_opCode == OP_CODE_EXIT) {
        return 0;
    } else if (!isInitialized) {
        std::cout << "not initialized!\n";
        std::string in;
        std::cin >> in;
        return 0;
    }

    return QCoreApplication::exec();
}

int main(int argc, char *argv[]) {
    QTNetworkDemo demo(argc, argv);
    demo.printWelcome();
    demo.askOperation();
    return demo.exec();
}
