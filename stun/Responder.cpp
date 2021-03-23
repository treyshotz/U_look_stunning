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

/**
 * Creates a message object and fills the necessary information
 *
 * @param transactionId from the request send from the client
 * @param addr, client address
 * @return message prepared for sending
 */
Message Responder::buildMessage(std::vector<uint32_t> transactionId, sockaddr_in addr) {
    Message message{};

    setHeader(message);
    setTransactionId(message, transactionId);
    setSoftware(message);
    setXorAdress(message, addr);
    //These two are not currently implemented
    //setHmacIntegrity(message);
    //setFingerprint(message);
    return message;
}

/**
 * Sets the header of a message
 * The header contains; type, length and cookie
 * @param message which will be parsed into
 */
void Responder::setHeader(Message &message) {
    //The length is the amount of bytes after transaction ID
    //Todo: The length is per now set static but it should be dynamic
    unsigned char responseAndLengthArray[] = "\x01\x01\x00\x20";
    uint32_t responseAndLength =
            (char) responseAndLengthArray[0] << 24 | responseAndLengthArray[1] << 16 | responseAndLengthArray[2] << 8 |
            responseAndLengthArray[3];

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
    //TODO: Better error handling
    if (transactionId.empty()) {
        std::cout << "Something went wrong during setting transactionId" << std::endl;
        return;
    }

    for (int i = 0; i < 3; i++) {
        message.setTransactionId(transactionId[i], i);
    }
}


/**
 * Set the sofware header and server name into a message
 * @param message to set the variables to
 */
void Responder::setSoftware(Message &message) {
    char serverName[] = "U look stunning";
    //Padding for the server name
    serverName[15] = '\x20';
    uint32_t headerAndLength;
    uint32_t c[4];

    char length = sizeof(serverName);
    char header[] = "\x80\x22";
    headerAndLength = (char) header[0] << 24 | header[1] << 16 | length;

    for (int i = 0; i < 4; i++) {
        c[i] = (char) serverName[i * 4] << 24 | serverName[i * 4 + 1] << 16 | serverName[i * 4 + 2] << 8 |
               serverName[i * 4 + 3];
    }

    message.setSoftwareHeader(headerAndLength);
    message.setServerName(c[0], 0);
    message.setServerName(c[1], 1);
    message.setServerName(c[2], 2);
    message.setServerName(c[3], 3);
}


/**
 * Sets the xor'd ip address from the client on the message
 * @param message to set the variables to
 * @param addr connection information from the client
 */
void Responder::setXorAdress(Message &message, sockaddr_in addr) {

    uint32_t ip = ntohl(addr.sin_addr.s_addr);
    uint16_t port = htons(addr.sin_port);

    unsigned char header[] = "\x00\x20\x00\x08";
    uint32_t head = (char) header[0] << 24 | header[1] << 16 | header[2] << 8 | header[3];


    //Convert to right endian
    std::bitset<32> a(ip);
    std::bitset<32> b;

    std::bitset<16> x(port);
    std::bitset<16> y;

    //Magic cookie
    std::bitset<32> z(std::string("00100001000100101010010001000010"));

    //Xor the port with the 16 most significant bits from the magic cookie
    int j = 32;
    int k = 16;
    for (int i = 0; i < 16; i++) {
        y[k - i] = z[j - i] ^ x[k - i];
    }

    //Xor the ip address with the magic cookie
    b = a ^ z;

    uint16_t xorPort = y.to_ullong();
    uint32_t xorIp = b.to_ullong();

    message.setXorHeader(head);
    message.setXorFamPort(xorPort);
    message.setXorIp(xorIp);
}


/**
 * NB! Not implemented
 *
 * @param message
 */
void Responder::setHmacIntegrity(Message &message) {
    std::cout << "HMAC NOT FINISHED" << std::endl;
}


/**
 * NB! Not implemented
 *
 * @param message
 */
void Responder::setFingerprint(Message &message) {

}
