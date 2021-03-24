//
// Created by Mads Lundegaard on 17/03/2021.
//

#ifndef STUN_READER_H
#define STUN_READER_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <bitset>
#include "Message.h"
#include "Responder.h"
#include "Reader.h"

class Reader {
public:
    std::vector<uint32_t> validateData(uint8_t *data, uint32_t datasize);

    uint8_t read8(std::vector<uint8_t> &collectedData);

    uint16_t read16(std::vector<uint8_t> &collectedData);

    uint32_t read32(std::vector<uint8_t> &collectedData);

    bool cookieChecker(uint32_t cookie);

    void messageChecker(std::vector<uint8_t> &collectedData, Message message);

    bool checkUsername(std::vector<uint8_t> &collectedData, uint16_t length);

    bool checkMsgIngrty(std::vector<uint8_t> &collectedData, uint16_t length);

    //These are the different attributes we found on all three of the stun pages made by ietf
    //Not all of these will be completely implemented due to lack of time
    enum AttributeType {
        //Comprehension-required range (0x0000-0x7FFF):
        TYPE_NONE = 0x0000,
        MAPPED_ADDR = 0x0001,
        CHANGE_REQ = 0x0003,
        USERNAME = 0x0006,
        MESSAGE_INTEGRITY = 0x0008,
        ERR_CODE = 0x0009,
        UNKNOWN_ATTRIBUTES = 0x000a,
        CHANNEL_NUMBER = 0x000c,
        LIFETIME = 0x000d,
        XOR_PEER_ADDR = 0x0012,
        DATA = 0x0013,
        REALM = 0x0014,
        NONCE = 0x0015,
        XOR_RELAY_ADDRESS = 0x0016,
        REQ_ADDRESS_FAMILY = 0x0017,
        EVEN_PORT = 0x0018,
        REQUESTED_TRANSPORT = 0x0019,
        DONT_FRAGMENT = 0x001a,
        XOR_MAPPED_ADDRESS = 0x0020,
        RESERVATION_TOKEN = 0x0022,
        PRIORITY = 0x0024,
        USE_CANDIDATE = 0x0025,
        PADDING = 0x0026,
        RESPONSE_PORT = 0x0027,

        //Comprehension-optional range (0x8000-0xFFFF)
        SOFTWARE = 0x8022,
        ALTERNATE_SERVER = 0x8023,
        FINGERPRINT = 0x8028,
        ICE_CONTROLLED = 0x8029,
        ICE_CONTROLLING = 0x802a,
        RESPONSE_ORIGIN = 0x802b,
        OTHER_ADDRESS = 0x802c,
    };

};
#endif //STUN_READER_H
