//
// Created by Mads Lundegaard on 16/03/2021.
//


#include <iostream>
#include <vector>
#include <bitset>
#include "Message.h"
#include "Reader.h"


/**
 * Validates the cookie
 * @param cookie
 * @return
 */
bool Reader::cookieChecker(uint32_t cookie) {
    //This is the cookie in binary
    std::bitset<32> a(std::string("00100001000100101010010001000010"));
    std::bitset<32> b(cookie);

    if (a == b)
        return true;
    return false;
}


/**
 * NB! This function is per now not in use as our stun server has no form for authentication!
 *
 *
 * Checks if a username is saved on the server
 * @param collectedData, vector containing the message sent
 * @param length, length of the username
 * @return true if username exists
 */
bool Reader::checkUsername(std::vector<uint8_t> &collectedData, uint16_t length) {
    int a = length;
    std::string inputUsername = "";
    for (int i = 0; i < a; i++) {
        inputUsername += collectedData[i];
    }
    //To find amount of bytes to delete
    int b = 4 - (length % 4);
    int z = length + b;
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

/**
 * NB! This function is not implemented or complete
 *
 *
 * @param collectedData
 * @param length
 * @return
 */
bool Reader::checkMsgIngrty(std::vector<uint8_t> &collectedData, uint16_t length) {
    return false;
}


/**
 * NB! This function is not implemented or complete
 * Due to the fact the we actually don't need to validate the request received, we didn't prioritize this
 *
 *
 * Checks the rest of the message for which atrributes exists in the message
 * @param collectedData, Vector with rest of the message
 */
void Reader::messageChecker(std::vector<uint8_t> &collectedData, Message message) {
    uint16_t type;
    uint16_t length;
    int teller = collectedData.size();

    while (teller >= 4) {

        //Due to lack of time we can't implement all of the different attributes
        type = read16(collectedData);
        length = read16(collectedData);
        std::cout << "lengde: " << length << std::endl;

        int b = 4 - (length % 4);
        int z = length + b;
        teller = teller - z;

        switch (type) {
            case USERNAME: {
                if (!(checkUsername(collectedData, length))) {
                    std::cout << "Username does not exist on server." << std::endl;
                }
                break;
            }

            case MESSAGE_INTEGRITY: {
                //With the exception of the FINGERPRINT
                //attribute, which appears after MESSAGE-INTEGRITY, agents MUST ignore
                //all other attributes that follow MESSAGE-INTEGRITY.
                checkMsgIngrty(collectedData, length);
                collectedData.erase(collectedData.begin(), collectedData.begin() + length);
                break;
            }

            case FINGERPRINT: {
                collectedData.erase(collectedData.begin(), collectedData.begin() + length);
                break;
            }


            default: {
                //If the data does not match a case it still removes the data so it can proceed
                collectedData.erase(collectedData.begin(), collectedData.begin() + length);
                break;
            }
        }
    }
}


/**
 * Takes in the request sent to the stun server and validates it
 * @param data, the data that will be validated
 * @param datasize, the size of the data that was sent
 */
std::vector<uint32_t> Reader::validateData(uint8_t *data, uint32_t datasize) {
    //Develop based on the example given by ietf , https://tools.ietf.org/html/rfc5769
    std::cout << "Validating data" << std::endl;
    std::vector<uint32_t> transID;

    //Store the data from input here
    std::vector<uint8_t> collectedData;

    //Checks that data and datasize is present
    //TODO: Is it right to return the vector?
    if (!data || !datasize) {
        std::cout << "wrong with input data" << std::endl;
        return transID;
    }

    //Copies the data into collectedData vector
    std::copy(data, data + datasize, std::back_inserter(collectedData));

    //TODO: Not sure if this works
    //TODO: Is it right to return the vector?

    //Check if the first byte and hexidecimal C0 (1100 0000), aka the two first bits is not 0
    if ((data[0] & 0xC0) != 0x00) {
        std::cout << "The first bits states that this is not a STUN request" << std::endl;
        return transID;
    }

    //These two variable are not actually used. But at least now we have control over what the read is doing
    uint16_t type = read16(collectedData);
    uint16_t length = read16(collectedData);
    uint32_t cookie = read32(collectedData);

    if (!cookieChecker(cookie)) {
        std::cout << "cookie was not right" << std::endl;
        return transID;
    }

    //Transaction ID is saved for sending back
    for (int i = 0; i < 3; i++) {
        transID.emplace_back(read32(collectedData));
    }

    return transID;
}


/**
 * Reads the first 16 bits of a vector
 * Removes the read bits upon finishing reading
 * @param collectedData the vector to read from
 * @return 16 first bits
 */
uint16_t Reader::read16(std::vector<uint8_t> &collectedData) {
    //TODO: Error handling

    uint16_t result = ((uint16_t) collectedData[0] << 8) | collectedData[1];
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

    uint32_t result =
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