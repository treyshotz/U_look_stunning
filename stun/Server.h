//
// Created by Mads Lundegaard on 18/03/2021.
//

#ifndef STUN_SERVER_H
#define STUN_SERVER_H


class Server {
public:
    void startServer();
    static void* threadTask(int socket);

    };


#endif //STUN_SERVER_H
