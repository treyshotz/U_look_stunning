//
// Created by Mads Lundegaard on 18/03/2021.
//

#include "Server.h"
#include <sys/socket.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <libc.h>



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

void Server::startServer() {

    int port = 4040;
    int socketFd;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in server_addr, cli_addr;
    int n;
    pthread_t thread;

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) {
        std::cout << ("Error opening a port") << std::endl;
        return;
    }

    bzero((char * ) &server_addr, sizeof(server_addr));

    //AF_INET = TCP, UDP etc
    server_addr.sin_family = AF_INET;
    //I think this sets the address automatically to current host's address
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    //Tries to bind the socket, if it failed it will have a value below zero
    if(bind(socketFd, (struct sockaddr * ) &server_addr, sizeof(server_addr)) < 0) {
        std::cout << "Error binding the socket" << std::endl;
    }

    std::cout << "Server has started on port " << port  << std::endl;
    //I think 5 is backlog?
    listen(socketFd, 5);

    clilen = sizeof(cli_addr);

    //Here we assign to new threads
    while(true) {

        int newSocketFd;
        newSocketFd = accept(socketFd, (struct sockaddr *) &cli_addr, &clilen);
        if (newSocketFd < 0) {
            std::cout << "Error on accepting the new connection" << std::endl;
            return;
        }

        std::cout << "Got connection from " << inet_ntoa(cli_addr.sin_addr) << " on port "
                  << ntohs(cli_addr.sin_port) << std::endl;
        std::cout << "Assigning to new thread" << std::endl;
        pthread_create(&thread, 0, threadTask, newSocketFd);

    }
}

//TODO: Do we really need to take in the mnessage?
static void* threadTask(int socket) {
    char buffer[1024];
    read(socket, buffer, 1024);
    std::cout << buffer << std::endl;


}

int main() {
    Server server;
    server.startServer();
}