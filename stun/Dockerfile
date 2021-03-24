FROM gcc:latest

COPY . /usr/src/myapp

WORKDIR /usr/src/myapp

RUN g++ -pthread -o main Message.cpp Message.h Responder.cpp Responder.h Reader.cpp Reader.h Server.cpp Server.h

CMD ["./main"]