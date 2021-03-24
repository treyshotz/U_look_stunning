//
// Created by Mads Lundegaard on 17/03/2021.
//

#ifndef STUN_RESPONDER_H
#define STUN_RESPONDER_H


#include <cstdint>
#include <vector>
#include <netinet/in.h>
#include "Message.h"

class Responder {
public:
    Message buildMessage(std::vector<uint32_t> transactionId, sockaddr_in addr);

private:
    void setHeader(Message &message);
    void setTransactionId(Message &message, std::vector<uint32_t> transactionId);
    void setSoftware(Message &message);
    void setXorAdress(Message &message, sockaddr_in addr);
    void setHmacIntegrity(Message &message);
    void setFingerprint(Message &message);
};


#endif //STUN_RESPONDER_H
