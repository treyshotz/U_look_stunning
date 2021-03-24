FROM node:latest

WORKDIR /usr/src/app

COPY . /usr/src/app

RUN npm install express --save  &&  npm install -g peer

CMD peerjs --port 9000 --sslkey key.pem --sslcert cert.pem
