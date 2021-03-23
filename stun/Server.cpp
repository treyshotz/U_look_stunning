//
// Created by Mads Lundegaard on 18/03/2021.
//

#include "Server.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <libc.h>


//TODO: Is it right to set this limit on the buffer?
struct args {
    sockaddr_in from;
    unsigned char buffer[1024];
    socklen_t fromlen;
};

int socketFd;
int length;

/*
   *
   * Flow:
   * Start a socket, making it possible for a client to connect
   * Assign this socket to a new thread which handles this socket
   * The thread start working on its tasks while the startServer method creates a new socket for connection
   * The thread uses methods from the Reader class to read and process the request
   * The thread keeps the transaction id for the incoming request and then proceeds to create a new message
   * This message will contain the same transaction id that it received but will also contain xor'd network information
   * After the message has been created the thread will make sure the message is sent back to the client
   * After sending the thread will finish and join the parent thread again
   */

//void Server::startServer() {
//
//    int port = 4040;
//    int socketFd;
//    socklen_t clilen;
//    struct sockaddr_in server_addr, cli_addr;
//    pthread_t thread;
//
//    //SOCK_DRGAM is for UDP
//    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
//    if (socketFd < 0) {
//        std::cout << ("Error opening a port") << std::endl;
//        return;
//    }
//
//    bzero((char * ) &server_addr, sizeof(server_addr));
//    memset(&server_addr, 0, sizeof(server_addr));
//    memset(&cli_addr, 0, sizeof(cli_addr));
//
//    //AF_INET = TCP, UDP etc
//    server_addr.sin_family = AF_INET;
//    //I think this sets the address automatically to current host's address
//    server_addr.sin_addr.s_addr = INADDR_ANY;
//    server_addr.sin_port = htons(port);
//
//    //Tries to bind the socket, if it failed it will have a value below zero
//    if(bind(socketFd, (struct sockaddr * ) &server_addr, sizeof(server_addr)) < 0) {
//        std::cout << "Error binding the socket" << std::endl;
//        return;
//    }
//
//    printf("IP address is: %s\n", inet_ntoa(server_addr.sin_addr));
//    std::cout << "Server has started on port " << port  << std::endl;
//    //I think 5 is backlog?
//    listen(socketFd, 5);
//
//    clilen = sizeof(cli_addr);
//
//    //Here we assign to new threads
//    while(true) {
//
//        int newSocketFd;
//        char buf[1024];
//        newSocketFd = recvfrom(socketFd, buf, 1024, MSG_WAITALL, (struct sockaddr*) &cli_addr,
//                               reinterpret_cast<socklen_t *>(1024));
//
//        //This is for TCP...
//        //newSocketFd = accept(socketFd, (struct sockaddr *) &cli_addr, &clilen);
////        if (newSocketFd < 0) {
////            std::cout << "Error on accepting the new connection" << std::endl;
////            return;
////        }
////
//        std::cout << buf << std::endl;
//
//        std::cout << "Got connection from " << inet_ntoa(cli_addr.sin_addr) << " on port "
//                  << ntohs(cli_addr.sin_port) << std::endl;
//        std::cout << "Assigning to new thread" << std::endl;
//        //What the F is this
//        pthread_create(&thread, 0, reinterpret_cast<void *(*)(void *)>(threadTask),
//                       reinterpret_cast<void *>(newSocketFd));
//        pthread_detach(thread);
//
//        std::cout << "Detached thread. Creating a new" << std::endl;
//    }
//}

/**
 * This starts the server and sets up UDP connection
 * When a data is received it gives the data to a separate thread to handle it
 * The thread validates the message, creates a response and sends it back
 * While the thread does it's tasks the loop can receive more requests
 */
[[noreturn]] void Server::startServer() {
    int SIZE = 1024;
    int PORT = 4040;

    unsigned char buffer[SIZE];
    struct sockaddr_in server_addr, cli_addr;
    pthread_t thread;

    //SOCK_DGRAM is for UDP
    if ((socketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cout << "Error opening a port" << std::endl;
        exit(132);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&cli_addr, 0, sizeof(cli_addr));

    // Filling server information
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    //Tries to bind the socket, if it failed it will have a value below zero
    if (bind(socketFd, (const struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        std::cout << "Error binding the socket" << std::endl;
        exit(132);
    }

    std::cout << "Server has started on port " << PORT << std::endl;

    length = sizeof(cli_addr); //length is value/result

    //Here we assign to the new threads
    int j = 0;
    while (true) {
        j++;
        std::cout << "Request " << j << std::endl;

        struct args *args = (struct args *) malloc(sizeof(struct args));
        //TODO: Do we need to free this buffer?
        recvfrom(socketFd, (char *) buffer, SIZE, MSG_WAITALL, (struct sockaddr *) &cli_addr,
                               reinterpret_cast<socklen_t *>(&length));
        std::cout << "Received data... Assigning to separate thread" << std::endl;


        //Use a struct to pass multiple variables into the thread function
        args->from = cli_addr;
        args->fromlen = sizeof(struct sockaddr_in);
        //This sucks. But it works
        for (int i = 0; i < 1024; i++) {
            args->buffer[i] = buffer[i];
        }

        //Assigning to a new thread and detatching thread
        pthread_create(&thread, 0, reinterpret_cast<void *(*)(void *)>(threadTask), (void *) args);
        pthread_detach(thread);
    }
}


void *Server::threadTask(args *input) {
    Reader reader;
    Responder responder;

    //Validate that this is not 0?
    std::vector<u_int32_t> transID = reader.validateData(((struct args *) input)->buffer, 1024);

    if (transID.empty()) {
        //TODO: Send back error message to the client
        //If the message is a request, the server MUST reject the request
        //with an error response.  This response MUST use an error code
        //of 401 (Unauthorized).
        std::cout << "Something went wrong during validating" << std::endl;
        std::cout << "Exiting thread" << std::endl;
        pthread_exit(0);
    }

    std::cout << "Check went okay. Preparing response" << std::endl;
    Message message = responder.buildMessage(transID, input->from);

    //TODO: Kinda dirty, try to find a better method? Also find a better way to handle the size
    int size = 13;
    uint32_t ab[size];
    for (int i = 0; i < size; i++) {
        ab[i] = message.SendPrep()[i];
        ab[i] = ntohl(ab[i]);
    }

    //Sends back the stun response
    int n = sendto(socketFd, ab, sizeof(ab), 0, (struct sockaddr*) &input->from , length);
    if (n < 0) {
        std::cout << "Failed sending" << std::endl;
    } else
        std::cout << "Sent!" << std::endl;

    std::cout << "Exiting... " << std::endl;
    free(input);
    pthread_exit(0);
}

int main() {
    Server server;
    server.startServer();
}