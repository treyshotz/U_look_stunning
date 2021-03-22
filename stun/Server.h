//
// Created by Mads Lundegaard on 18/03/2021.
//

#ifndef STUN_SERVER_H
#define STUN_SERVER_H


class Server {
public:
    [[noreturn]] void startServer();
    static void* threadTask(void *input);

    };


#endif //STUN_SERVER_H
