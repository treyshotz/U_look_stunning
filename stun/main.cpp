#include <iostream>
#include <vector>
#include <iomanip>
#include <bitset>
#include "Message.h"

class Reader {
public:
    void validateData(uint8_t *data, uint32_t datasize);

    uint8_t read8(std::vector<uint8_t> &collectedData);

    uint16_t read16(std::vector<uint8_t> &collectedData);

    uint32_t read32(std::vector<uint8_t> &collectedData);

    bool cookieChecker(uint32_t cookie);

    void messageChecker(std::vector<uint8_t> &collectedData, Message message);

    bool checkUsername(std::vector<uint8_t> &collectedData, uint16_t length);

    bool checkMsgIngrty(std::vector<uint8_t> &collectedData, uint16_t length);

    bool checkXorMapped(std::vector<uint8_t> &collectedData, uint16_t length, uint32_t cookie);

    //These are the different attributes we found on all three of the stun pages made by ietf
    //Not all of these will be completely implemented due to lack of time
    enum AttributeType {
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
        SOFTWARE = 0x8022,
        ALTERNATE_SERVER = 0x8023,
        FINGERPRINT = 0x8028,
        ICE_CONTROLLED = 0x8029,
        ICE_CONTROLLING = 0x802a,
        RESPONSE_ORIGIN = 0x802b,
        OTHER_ADDRESS = 0x802c,
    };

};

/**
 * Validates the cookie
 * @param cookie
 * @return
 */
bool Reader::cookieChecker(uint32_t cookie) {
    std::bitset<32> a(std::string("00100001000100101010010001000010"));
    std::bitset<32> b(cookie);

    if (a == b)
        return true;
    return false;
}


/**
 * Checks if a username is saved on the server
 * @param collectedData, vector containing the message sent
 * @param length, length of the username
 * @return true if username exists
 */
bool Reader::checkUsername(std::vector<uint8_t> &collectedData, uint16_t length) {
    //TODO: Is this the right way to do it? What about long term?
    int a = length;
    std::string inputUsername = "";
    for (int i = 0; i < a; i++) {
        inputUsername += collectedData[i];
    }
    //To find amount of bytes to delete
    int z = (length + (4 - (length % 4))) / 4;
    collectedData.erase(collectedData.begin(), collectedData.begin() + z);

    //As you can see, security is not our biggest priority in this task
    const char *users[] = {"madslun", "simonje", "larsbost"};
    //That O(n) function
    for (auto &a : users) {
        if (inputUsername == a)
            return true;
    }

    return false;
}


bool Reader::checkMsgIngrty(std::vector<uint8_t> &collectedData, uint16_t length) {
    return false;
}


bool Reader::checkXorMapped(std::vector<uint8_t> &collectedData, uint16_t length, uint32_t cookie) {
    uint16_t xxxandFamily = read16(collectedData);
    std::bitset<16> a(xxxandFamily);
    std::cout << a << std::endl;
    uint16_t xPort = read16(collectedData);

    std::bitset<16> x(xPort);
    std::bitset<32> z(cookie);
    std::bitset<16> y;
    //std::cout << x << '\n';
    //std::cout << z << '\n';


    int j = 32;
    int k = 16;
    for (int i = 0; i < 16; i++) {
        y[k - i] = z[j - i] ^ x[k - i];
    }
    std::cout << y << std::endl;

    //TODO: Convert the xor'd bits to "network byte order"
    // find out whether this means big endian or little endian.


    return false;
}


/**
 * Checks the rest of the message for which atrributes exists in the message
 * @param collectedData, Vector with rest of the message
 */
void Reader::messageChecker(std::vector<uint8_t> &collectedData, Message message) {

    uint16_t type;
    uint16_t length;
    bool a = true;

    //TODO: change while loop to iterate only until the message is finished
    while (a) {
        //Due to lack of time we can't implement all of the different attributes

        type = read16(collectedData);
        length = read16(collectedData);


        switch (type) {
            case USERNAME: {
                //TODO: username stuff check here
                if (!(checkUsername(collectedData, length))) {
                    std::cout << "Username does not exist on server." << std::endl;
                    a = false;
                    //If the message is a request, the server MUST reject the request
                    //with an error response.  This response MUST use an error code
                    //of 401 (Unauthorized).

                }
                break;
            }

            case MESSAGE_INTEGRITY: {
                //TODO: Integrity stuff here
                //With the exception of the FINGERPRINT
                //attribute, which appears after MESSAGE-INTEGRITY, agents MUST ignore
                //all other attributes that follow MESSAGE-INTEGRITY.
                break;
            }

            case FINGERPRINT: {
                //TODO: Fingerprint stuff here
                break;
            }

            //TODO: I think this is only set by the server
            case XOR_MAPPED_ADDRESS: {
                std::cout << "XOR skjer her" << std::endl;
                printf("type %02hx\n", type);
                for (auto& el : collectedData)
                    printf("%02hhx ", el);
                std::cout << '\n';
                checkXorMapped(collectedData, length, message.getCookie());
                break;
            }

            default: {
                std::cout << "default will come here" << std::endl;
                break;
            }
        }
    }

    /*
     Comprehension-required range (0x0000-0x7FFF):
     0x0000: (Reserved)
     0x0001: MAPPED-ADDRESS
     0x0002: (Reserved; was RESPONSE-ADDRESS)
     0x0003: (Reserved; was CHANGE-ADDRESS)
     0x0004: (Reserved; was SOURCE-ADDRESS)
     0x0005: (Reserved; was CHANGED-ADDRESS)
     0x0006: USERNAME
     0x0007: (Reserved; was PASSWORD)
     0x0008: MESSAGE-INTEGRITY
     0x0009: ERROR-CODE
     0x000A: UNKNOWN-ATTRIBUTES
     0x000B: (Reserved; was REFLECTED-FROM)
     0x0014: REALM
     0x0015: NONCE
     0x0020: XOR-MAPPED-ADDRESS

   Comprehension-optional range (0x8000-0xFFFF)
     0x8022: SOFTWARE
     0x8023: ALTERNATE-SERVER
     0x8028: FINGERPRINT
         */

}


//TODO: Take in the request
//  Parse the actual data?
/**
 * Takes in the request sent to the stun server and parses it
 * @param data, the data that will be validated and parsed
 * @param datasize, the size of the data that was sent
 */
void Reader::validateData(uint8_t *data, uint32_t datasize) {
//Develop based on the example given by ietf , https://tools.ietf.org/html/rfc5769

//TODO: Because of the hmac sha1 fingerprint we need to store the message somewhere instead of deleting it

//Store the data from input here
    std::vector<uint8_t> collectedData;

//Create a message for sending it back
    Message message{};

//Checks that data and datasize is present
    if (!data || !datasize) {
        std::cout << "wrong with input data" << std::endl;
        return;
    }

//Copies the data into collectedData vector
    std::copy(data, data + datasize, std::back_inserter(collectedData));

//Check if the first byte and hexidecimal C0 (1100 0000), aka the two first bits is not 0
    if ((data[0] & 0xC0) != 0x00) {
        return;
    }

    //uint16_t type = read16(collectedData);
    //uint16_t length = read16(collectedData);
    //uint32_t cookie = read32(collectedData);
    message.setType(read16(collectedData));
    message.setLength(read16(collectedData));
    message.setCookie(read32(collectedData));

    //Transaction ID is saved for sending back
    uint32_t transID[3];
    for (int i = 0; i < 3; i++) {
        transID[i] = read32(collectedData);
    }

    if (!cookieChecker(message.getCookie())) {
        std::cout << "cookie was not right" << std::endl;
        return;
    }

    messageChecker(collectedData, message);

/*

    std::bitset<16> x(type);
    std::cout << x << '\n';
    std::bitset<16> z(length);
    std::cout << z << '\n';
    std::bitset<32> y(cookie);
    std::cout << y << '\n';


   for (auto& el : collectedData)
       printf("%02hhx", el);
   std::cout << '\n';
*/
}


/**
 * Reads the first 16 bits of a vector
 * Removes the read bits upon finishing reading
 * @param collectedData the vector to read from
 * @return 16 first bits
 */
uint16_t Reader::read16(std::vector<uint8_t> &collectedData) {
    //TODO: Error handling

    uint16_t result = 0;
    result = ((uint16_t) collectedData[0] << 8) | collectedData[1];
    collectedData.erase(collectedData.begin(), collectedData.begin() + 2);
    return result;
}


/**
 * Reads the first 32 bits of a vector
 * Removes the read bits upon finishing reading
 * @param collectedData the vector to read from
 * @return 32 first bits
 */
uint32_t Reader::read32(std::vector<uint8_t> &collectedData) {
    //TODO: Error handling

    uint32_t result = 0;
    result =
            ((uint16_t) collectedData[0] << 24) | (collectedData[1] << 16) | (collectedData[2] << 8) | collectedData[3];
    collectedData.erase(collectedData.begin(), collectedData.begin() + 4);
    return result;
}


/**
 * Reads the first 8 bits of a vector
 * Removes the read bits upon finishing reading
 * @param collectedData, the vector to read from
 * @return 8 first bits
 */
uint8_t Reader::read8(std::vector<uint8_t> &collectedData) {
    //TODO: Error handling
    return collectedData[0];
}


int main() {
    //TODO: Move this to a seperate test file
    /**
     * This request uses the following parameters:
       Software name:  "STUN test client" (without quotes)
       Username:  "evtj:h6vY" (without quotes)
       Password:  "VOkJxbRl1RmTxUk/WvJxBt" (without quotes)

       00 01 00 58     Request type and message length
       21 12 a4 42     Magic cookie
       b7 e7 a7 01  }
       bc 34 d6 86  }  Transaction ID
       fa 87 df ae  }
       80 22 00 10     SOFTWARE attribute header
       53 54 55 4e  }
       20 74 65 73  }  User-agent...
       74 20 63 6c  }  ...name
       69 65 6e 74  }
       00 24 00 04     PRIORITY attribute header
       6e 00 01 ff     ICE priority value
       80 29 00 08     ICE-CONTROLLED attribute header
       93 2f f9 b1  }  Pseudo-random tie breaker...
       51 26 3b 36  }   ...for ICE control
       00 06 00 09     USERNAME attribute header
       65 76 74 6a  }
       3a 68 36 76  }  Username (9 bytes) and padding (3 bytes)
       59 20 20 20  }
       00 08 00 14     MESSAGE-INTEGRITY attribute header
       9a ea a7 0c  }
       bf d8 cb 56  }
       78 1e f2 b5  }  HMAC-SHA1 fingerprint
       b2 d3 f2 49  }
       c1 b5 71 a2  }
       80 28 00 04     FINGERPRINT attribute header
       e5 7a 3b cf     CRC32 fingerprint
    */

    const unsigned char req[] =
            "\x00\x01\x00\x58"
            "\x21\x12\xa4\x42"
            "\xb7\xe7\xa7\x01\xbc\x34\xd6\x86\xfa\x87\xdf\xae"
            "\x80\x22\x00\x10"
            "STUN test client"
            "\x00\x24\x00\x04"
            "\x6e\x00\x01\xff"
            "\x80\x29\x00\x08"
            "\x93\x2f\xf9\xb1\x51\x26\x3b\x36"
            "\x00\x06\x00\x07"
            "\x6d\x61\x64\x73\x6c\x75\x6e\x20"
            "\x00\x08\x00\x14"
            "\x9a\xea\xa7\x0c\xbf\xd8\xcb\x56\x78\x1e\xf2\xb5"
            "\xb2\xd3\xf2\x49\xc1\xb5\x71\xa2"
            "\x80\x28\x00\x04"
            "\xe5\x7a\x3b\xcf";

    Reader reader;
    reader.validateData((uint8_t *) req, sizeof(req));

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
