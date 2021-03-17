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

    const uint32_t *getTransactionId() const;

    void setTransactionId(uint32_t t, int i);

    uint32_t getSoftwareHeader() const;

    void setSoftwareHeader(uint32_t softwareHeader);

    void setServerName(uint32_t t, int i);

    const uint32_t *getServerName() const;

    uint32_t getXorHeader() const;

    void setXorHeader(uint32_t xorHeader);

    const uint32_t *getXorAdress() const;

    uint32_t getIntegrityHeader() const;

    void setIntegrityHeader(uint32_t integrityHeader);

    const uint32_t *getHmacFingerprint() const;

    uint32_t getFingerprintHeader() const;

    void setFingerprintHeader(uint32_t fingerprintHeader);

    uint32_t getFingerprint() const;

    void setFingerprint(uint32_t fingerprint);


    /*01 01 00 3c     Response type and message length
     21 12 a4 42     Magic cookie
     b7 e7 a7 01  }
     bc 34 d6 86  }  Transaction ID
     fa 87 df ae  }
     80 22 00 0b     SOFTWARE attribute header
     74 65 73 74  }
     20 76 65 63  }  UTF-8 server name
     74 6f 72 20  }
     00 20 00 08     XOR-MAPPED-ADDRESS attribute header
     00 01 a1 47     Address family (IPv4) and xor'd mapped port number
     e1 12 a6 43     Xor'd mapped IPv4 address
     00 08 00 14     MESSAGE-INTEGRITY attribute header
     2b 91 f5 99  }
     fd 9e 90 c3  }
     8c 74 89 f9  }  HMAC-SHA1 fingerprint
     2a f9 ba 53  }
     f0 6b e7 d7  }
     80 28 00 04     FINGERPRINT attribute header
     c0 7d 4c 96     CRC32 fingerprint
     */
public:
    std::uint16_t type;
    std::uint16_t length;
    std::uint32_t cookie;
    std::uint32_t transactionID[3];
    std::uint32_t softwareHeader;
    //TODO: I dont think this is the right size
    std::uint32_t serverName[3];
    std::uint32_t xorHeader;
    //TODO: This is only for ipv4. Add for ipv6 aswell
    std::uint32_t xorAdress[2];
    std::uint32_t integrityHeader;
    std::uint32_t hmacFingerprint[5];
    std::uint32_t fingerprintHeader;
    std::uint32_t fingerprint;

};


#endif //STUN_MESSAGE_H
