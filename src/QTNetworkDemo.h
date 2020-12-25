//
//  QTNetworkDemo.h
//  QTNetworkDemo
//
//  Created by mugx on 04/03/12.
//  Copyright (c) 2012-2020 mugx. All rights reserved.
// 

#ifndef QTNetworkDemo_h
#define QTNetworkDemo_h

#include <QtCore/QCoreApplication>
#include <iostream>
#include <string>
#include <sstream>

class QTNetworkDemo : public QCoreApplication {
public:
    enum OP_CODE {
        OP_CODE_ERROR = -1,
        OP_CODE_NOP = 0,
        OP_CODE_CLIENT_SELECTED = 1,
        OP_CODE_SERVER_SELECTED = 2,
        OP_CODE_EXIT = 3

    };

    QTNetworkDemo(int argc, char *argv[]);
    void printWelcome();
    void askOperation();
    int exec();

private:
    const std::string CLIENT_CHOICE_STR;
    const std::string SERVER_CHOICE_STR;
    const std::string EXIT_CHOICE_STR;

    OP_CODE m_opCode;
};

#endif
