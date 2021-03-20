//
// Created by Mads Lundegaard on 17/03/2021.
//

#include <cstring>
#include <cstdio>
#include "Responder.h"
#include <cinttypes>
#include <bitset>
#include <iostream>
#include <zlib.h>

Message Responder::buildMessage(uint32_t *transactionId) {
    Message message{};

    setHeader(message);
    setTransactionId(message, transactionId);
    setSoftware(message);
    setXorAdress(message);
    setHmacIntegrity(message);
    setFingerprint(message);

    return message;
}

/**
 * Sets the header of a message
 * The header contains; type, length and cookie
 * @param message which will be parsed into
 */
void Responder::setHeader(Message &message) {
    //The length is the amount of bytes after transaction ID
    //On a ipv4 response it should be 64
    char responseAndLengthArray[] = "\x01\x01\x00\x40";
    uint32_t responseAndLength = (char) responseAndLengthArray[0] << 24 | responseAndLengthArray[1] << 16 | responseAndLengthArray[2] << 8 | responseAndLengthArray[3];

    char cookieArr[] = "\x21\x12\a4\42";
    uint32_t cookie = (char) cookieArr[0] << 24 | cookieArr[1] << 16 | cookieArr[2] << 8 | cookieArr[4];

    message.setTypeAndLength(responseAndLength);
    message.setCookie(cookie);
}


/**
 * Sets the transactionId on a message
 * @param message to set the transactionID on
 * @param transactionId receives the transactionID from received request
 */
void Responder::setTransactionId(Message &message, uint32_t transactionId[]) {
    //TODO: Validate that array is not empty
    if(transactionId)
    for(int i = 0; i < 3; i++) {
        message.setTransactionId(transactionId[i], i);
    }
}


/**
 * Set the sofware header and server name into a message
 * @param message to the the variables to
 */
void Responder::setSoftware(Message &message) {
    char serverName[] = "U look stunning";
    //Padding for the server name
    serverName[15] = '\x20';
    uint32_t headerAndLength = 0;
    uint32_t c[5];

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
    printf("%02x ", c[3]);
}


//TODO: Take network as input
void Responder::setXorAdress(Message &message) {
    /*
    uint16_t xxxandFamily = read16(collectedData);
    std::bitset<16> a(xxxandFamily);
    std::cout << a << std::endl;
    uint16_t xPort = read16(collectedData);

    std::bitset<16> x(xPort);
    std::bitset<32> z(std::string("00100001000100101010010001000010"));
    std::bitset<16> y;
    //std::cout << x << '\n';
    //std::cout << z << '\n';


    int j = 32;
    int k = 16;
    for (int i = 0; i < 16; i++) {
        y[k - i] = z[j - i] ^ x[k - i];
    }
    std::cout << y << std::endl;
*/
    //TODO: Convert the xor'd bits to "network byte order"
    // find out
}

void Responder::setHmacIntegrity(Message &message) {
    std::cout << "HMAC NOT FINISHED" << std::endl;

}

void Responder::setFingerprint(Message &message) {
    std::cout << "FINGERPRINT NOT FINSHED" << std::endl;
}
