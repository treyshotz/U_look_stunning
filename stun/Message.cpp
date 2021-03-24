//
// Created by Mads Lundegaard on 17/03/2021.
//

#include "Message.h"
#include <iostream>


std::vector<uint32_t> Message::SendPrep() {

    //TODO: set right size
    std::vector<uint32_t> msg;
    msg.reserve(13);
    //uint32_t msg[13];
    msg[0] = getTypeAndLength();
    msg[1] = getCookie();
    msg[2] = getTransactionId()[0];
    msg[3] = getTransactionId()[1];
    msg[4] = getTransactionId()[2];
    msg[5] = getSoftwareHeader();
    msg[6] = getServerName()[0];
    msg[7] = getServerName()[1];
    msg[8] = getServerName()[2];
    msg[9] = getServerName()[3];
    msg[10] = getXorHeader();
    msg[11] = getXorAdress()[0];
    msg[12] = getXorAdress()[1];
    //TODO: Add this.
//    msg[13] = getIntegrityHeader();
//    msg[14] = getHmacFingerprint()[0];
//    msg[15] = getHmacFingerprint()[1];
//    msg[16] = getHmacFingerprint()[2];
//    msg[17] = getHmacFingerprint()[3];
//    msg[18] = getHmacFingerprint()[4];
//    msg[19] = getFingerprintHeader();
//    msg[20] = getFingerprint();
    return msg;
}

uint32_t Message::getCookie() const {
    return cookie;
}

void Message::setCookie(uint32_t cookie) {
    Message::cookie = cookie;
}

const uint32_t *Message::getTransactionId() const {
    return transactionID;
}

uint32_t Message::getSoftwareHeader() const {
    return softwareHeader;
}

void Message::setSoftwareHeader(uint32_t softwareHeader) {
    Message::softwareHeader = softwareHeader;
}

const uint32_t *Message::getServerName() const {
    return serverName;
}

uint32_t Message::getXorHeader() const {
    return xorHeader;
}

void Message::setXorHeader(uint32_t xorHeader) {
    Message::xorHeader = xorHeader;
}

const uint32_t *Message::getXorAdress() const {
    return xorAdress;
}

uint32_t Message::getIntegrityHeader() const {
    return integrityHeader;
}

void Message::setIntegrityHeader(uint32_t integrityHeader) {
    Message::integrityHeader = integrityHeader;
}

const uint32_t *Message::getHmacFingerprint() const {
    return hmacFingerprint;
}

uint32_t Message::getFingerprintHeader() const {
    return fingerprintHeader;
}

void Message::setFingerprintHeader(uint32_t fingerprintHeader) {
    Message::fingerprintHeader = fingerprintHeader;
}

uint32_t Message::getFingerprint() const {
    return fingerprint;
}

void Message::setFingerprint(uint32_t fingerprint) {
    Message::fingerprint = fingerprint;
}

void Message::setTransactionId(uint32_t t, int i) {
    Message::transactionID[i] = t;
}

void Message::setServerName(uint32_t t, int i) {
    Message::serverName[i] = t;
}

uint32_t Message::getTypeAndLength() const {
    return typeAndLength;
}

void Message::setTypeAndLength(uint32_t typeAndLength) {
    Message::typeAndLength = typeAndLength;
}

void Message::setXorFamPort(uint16_t port) {
    uint32_t famAndPort;
    //TODO: x01 is only for ipv4... Maybe ipv6 should be added
    unsigned char zeroAndFam[] = "\x00\x01";
    famAndPort = (char) zeroAndFam[0] << 24 | zeroAndFam[1] << 16 | port;
    Message::xorAdress[0] = famAndPort;
}

void Message::setXorIp(uint32_t ip) {
    Message::xorAdress[1] = ip;
}



