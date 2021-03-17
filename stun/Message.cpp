//
// Created by Mads Lundegaard on 17/03/2021.
//

#include "Message.h"

uint16_t Message::getType() const {
    return type;
}

void Message::setType(uint16_t type) {
    Message::type = type;
}

uint16_t Message::getLength() const {
    return length;
}

void Message::setLength(uint16_t length) {
    Message::length = length;
}

uint32_t Message::getCookie() const {
    return cookie;
}

void Message::setCookie(uint32_t cookie) {
    Message::cookie = cookie;
}
