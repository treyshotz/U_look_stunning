#include <iostream>
#include <vector>
#include <iomanip>
#include <bitset>

class Reader {
public:
    void validateData(uint8_t* data, uint32_t datasize);

    uint8_t read8(std::vector<uint8_t> collectedData);

    uint16_t read16(std::vector<uint8_t> collectedData);

    uint32_t read32(std::vector<uint8_t> collectedData);

    bool cookieChecker(uint32_t cookie);
};

/**
 * Validates the cookie
 * @param cookie
 * @return
 */
bool Reader::cookieChecker(uint32_t cookie) {
    std::bitset<32> a(std::string("00100001000100101010010001000010"));
    std::bitset<32> b(cookie);
    if(a == b)
        return true;
    return false;
}

//TODO: Take in the request
//  Check the two first bits that it is 0x00
//  Parse 16 first bits to type
//  Parse 16 next bits to length
//  Parse 32 next bits to cookie
//  Parse 96 next bits to transaction id
//  Parse the actual data?
/**
 * Takes in the request sent to the stun server and parses it
 * @param data, the data that will be validated and parsed
 * @param datasize, the size of the data that was sent
 */
void Reader::validateData(uint8_t* data, uint32_t datasize) {
//Develop based on the example given by ietf , https://tools.ietf.org/html/rfc5769


//Store the data from input here
std::vector<uint8_t> collectedData;

//Checks that data and datasize is present
if(!data || !datasize) {
    std::cout << "wrong with input data" << std::endl;
    return;
}

//Copies the data into collectedData vector
std::copy(data, data+datasize, std::back_inserter(collectedData));

//Check if the first byte and hexidecimal C0 (1100 0000), aka the two first bits is not 0
    if ( (data[0] & 0xC0) != 0x00) {
        return;
    }
    //TODO: Plz use pointer instead or delete in a better way at least
    uint16_t type = read16(collectedData);
    collectedData.erase(collectedData.begin(), collectedData.begin()+2);
    uint16_t length = read16(collectedData);
    collectedData.erase(collectedData.begin(), collectedData.begin()+2);
    uint32_t cookie = read32(collectedData);
    collectedData.erase(collectedData.begin(), collectedData.begin()+4);

    if(!cookieChecker(cookie)) {
        std::cout << "cookie was not right" << std::endl;
    }


    /*std::bitset<16> x(type);
    std::cout << x << '\n';
    std::bitset<16> z(length);
    std::cout << z << '\n';
    std::bitset<32> y(cookie);
    std::cout << y << '\n';

    std::bitset<16> x(result);
   std::cout << x << '\n';


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
uint16_t Reader::read16(std::vector<uint8_t> collectedData) {
    //TODO: Error handling

    uint16_t result = 0;
    result = ((uint16_t)collectedData[0] << 8) | collectedData[1];
    return result;
}


/**
 * Reads the first 32 bits of a vector
 * Removes the read bits upon finishing reading
 * @param collectedData the vector to read from
 * @return 32 first bits
 */
uint32_t Reader::read32(std::vector<uint8_t> collectedData) {
    //TODO: Error handling

    uint32_t result = 0;
    result = ((uint16_t)collectedData[0] << 24) | (collectedData[1] << 16)| (collectedData[2] << 8) | collectedData[3];
    return result;
}


/**
 * Reads the first 8 bits of a vector
 * Removes the read bits upon finishing reading
 * @param collectedData, the vector to read from
 * @return 8 first bits
 */
uint8_t Reader::read8(std::vector<uint8_t> collectedData) {
    //TODO: Error handling
    return collectedData[0];
}

int main() {
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
            "\x00\x06\x00\x09"
            "\x65\x76\x74\x6a\x3a\x68\x36\x76\x59\x20\x20\x20"
            "\x00\x08\x00\x14"
            "\x9a\xea\xa7\x0c\xbf\xd8\xcb\x56\x78\x1e\xf2\xb5"
            "\xb2\xd3\xf2\x49\xc1\xb5\x71\xa2"
            "\x80\x28\x00\x04"
            "\xe5\x7a\x3b\xcf";

    Reader reader;
    reader.validateData((uint8_t*) req, sizeof(req));

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
