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

Message Responder::buildMessage(std::vector<uint32_t> transactionId, sockaddr_in addr) {
    Message message{};

    setHeader(message);
    setTransactionId(message, transactionId);
    setSoftware(message);
    setXorAdress(message, addr);
    setHmacIntegrity(message);
    setFingerprint(message);


}

/**
 * Sets the header of a message
 * The header contains; type, length and cookie
 * @param message which will be parsed into
 */
void Responder::setHeader(Message &message) {
    //The length is the amount of bytes after transaction ID
    //On a ipv4 response it should be 64
    unsigned char responseAndLengthArray[] = "\x01\x01\x00\x40";
    uint32_t responseAndLength = (char) responseAndLengthArray[0] << 24 | responseAndLengthArray[1] << 16 | responseAndLengthArray[2] << 8 | responseAndLengthArray[3];

    unsigned char cookieArr[] = "\x21\x12\xa4\x42";
    uint32_t cookie = (char) cookieArr[0] << 24 | cookieArr[1] << 16 | cookieArr[2] << 8 | cookieArr[3];

    message.setTypeAndLength(responseAndLength);
    message.setCookie(cookie);
}


/**
 * Sets the transactionId on a message
 * @param message to set the transactionID on
 * @param transactionId receives the transactionID from received request
 */
void Responder::setTransactionId(Message &message, std::vector<uint32_t> transactionId) {
    //TODO: Validate that array is not empty
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
    uint32_t c[4];

    char length = sizeof(serverName);
    char header[] =  "\x80\x22";
    headerAndLength = (char) header[0] << 24 | header[1] << 16 | length;

    for(int i = 0; i < 4; i++) {
        c[i] = (char) serverName[i*4] << 24 | serverName[i*4+1] << 16 | serverName[i*4+2] << 8 | serverName[i*4+3];
    }

    message.setSoftwareHeader(headerAndLength);
    message.setServerName(c[0], 0);
    message.setServerName(c[1], 1);
    message.setServerName(c[2], 2);
    message.setServerName(c[3], 3);
}


//TODO: Take network as input
void Responder::setXorAdress(Message &message, sockaddr_in addr) {

    uint32_t ip = ntohl(addr.sin_addr.s_addr);
    uint16_t port = htons(addr.sin_port);
    //printf("IP fra xor : %s\n", inet_ntoa(addr.sin_addr));
    printf("Port fra xor : %hu\n", port);

    unsigned char header[] = "\x00\x20\x00\x08";
    uint32_t head = (char) header[0] << 24 | header[1] << 16 | header[2] << 8 | header[3];


    //TODO: Maybe we need to convert this back?
    //Convert to right endian
    std::bitset<16> portBits(port);

    /*
    uint16_t xxxandFamily = read16(collectedData);
    std::bitset<16> a(xxxandFamily);
    std::cout << a << std::endl;
    uint16_t xPort = read16(collectedData);
*/
    std::bitset<32> a(ip);
    std::bitset<32> b;

    std::bitset<16> x(port);
    //Magic cookie
    std::bitset<32> z(std::string("00100001000100101010010001000010"));
    std::bitset<16> y;

    int j = 32;
    int k = 16;
    for (int i = 0; i < 16; i++) {
        y[k - i] = z[j - i] ^ x[k - i];
    }

    b = a ^ z;
    std::cout << b << std::endl;
    uint16_t xorPort = y.to_ullong();
    uint32_t xorIp = b.to_ullong();

    message.setXorHeader(head);
    message.setXorFamPort(xorPort);
    message.setXorIp(xorIp);
}

void Responder::setHmacIntegrity(Message &message) {

}

void Responder::setFingerprint(Message &message) {

}
