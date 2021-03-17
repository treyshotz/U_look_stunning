//
// Created by Mads Lundegaard on 17/03/2021.
//

#include <cstring>
#include <cstdio>
#include "Responder.h"
#include <cinttypes>

void Responder::buildMessage(std::uint32_t *transactionId) {
    Message message{};

    //setHeader(message);
    //setTransactionId(message, transactionId);
    setSoftware(message);
    //setXorAdress(message);
    //setHmacIntegrity(message);
    //setFingerprint(message);


}

void Responder::setHeader(Message &message) {

}

void Responder::setTransactionId(Message &message, uint32_t transactionId[]) {
    for(int i = 0; i < 3; i++) {
        message.setTransactionId(transactionId[i], i);
    }
}

void Responder::setSoftware(Message &message) {
    char serverName[] = "U look stunning";
    //Padding for the server name
    serverName[15] = '\x20';
    uint32_t headerAndLength = 0;
    uint32_t c[4];

    char length = (char) strlen(serverName);
    char header[] =  "\x80\x22";
    headerAndLength = (char) header[0] << 24 | header[1] << 16 | length;

    for(int i = 0; i < 4; i++) {
        c[i] = (char) serverName[i*4] << 24 | serverName[i*4+1] << 16 | serverName[i*4+2] << 8 | serverName[i*4+3];
    }

    //printf("%08" PRIx32 "\n", headerAndLength);
    //printf("%08" PRIx32 "\n", c[0]);
    //printf("%08" PRIx32 "\n", c[1]);
    //printf("%08" PRIx32 "\n", c[2]);
    //printf("%08" PRIx32 "\n", c[3]);

    message.setSoftwareHeader(headerAndLength);
    message.setServerName(c[0], 0);
    message.setServerName(c[1], 1);
    message.setServerName(c[2], 2);
    message.setServerName(c[3], 3);
}

void Responder::setXorAdress(Message &message) {

}

void Responder::setHmacIntegrity(Message &message) {

}

void Responder::setFingerprint(Message &message) {

}
