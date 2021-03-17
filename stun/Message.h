//
// Created by Mads Lundegaard on 17/03/2021.
//

#ifndef STUN_MESSAGE_H
#define STUN_MESSAGE_H


#include <cstdint>

class Message {
public:
    uint16_t getType() const;

    void setType(uint16_t type);

    uint16_t getLength() const;

    void setLength(uint16_t length);

    uint32_t getCookie() const;

    void setCookie(uint32_t cookie);

public:
    std::uint16_t type;
    std::uint16_t length;
    std::uint32_t cookie;

};


#endif //STUN_MESSAGE_H
