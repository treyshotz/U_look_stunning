cmake_minimum_required(VERSION 3.10)
project(stun)

set(CMAKE_CXX_STANDARD 14)

add_executable(stun main.cpp Message.cpp Message.h Responder.cpp Responder.h Reader.h Server.cpp Server.h)

