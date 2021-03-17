//
// Created by Mads Lundegaard on 17/03/2021.
//

#include "Message.h"

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

