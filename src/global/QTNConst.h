//
//  QTNConst.h
//  QTNetworkDemo
//
//  Created by mugx on 04/03/12.
//  Copyright (c) 2012-2020 mugx. All rights reserved.

#ifndef QTNConst_h
#define QTNConst_h

class QTNConst
{
public:
    static const char* SERVER_IP;
    static int SERVER_PORT;
    static int NUM_MAX_CLIENTS;
    static int SOCKET_TIMEOUT;
    static int TIMEOUT_FOR_SENDING_LOGOUT_MSG;

    static const char* MSG_WELCOME;
    static const char* MSG_LOGOUT;
    static const char* MSG_GOODBYE;

    static const char* CLIENT_CHOICE_STR;
    static const char* SERVER_CHOICE_STR;
    static const char* EXIT_CHOICE_STR;
};

#endif
