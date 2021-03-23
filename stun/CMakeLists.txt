cmake_minimum_required(VERSION 3.17)
project(stun)

set(CMAKE_CXX_STANDARD 20)

add_executable(stun Message.cpp Message.h Responder.cpp Responder.h Reader.cpp Reader.h Server.cpp Server.h)