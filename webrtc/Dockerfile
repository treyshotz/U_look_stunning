FROM node:latest

WORKDIR /usr/src/app

COPY ["package.json", "package-lock.json*", "./"]

COPY . .

EXPOSE 443

RUN npm install --save express  &&  npm install -g peer  && npm install -g nodemon


CMD cd cert && peerjs --port 9000 --sslkey key.pem --sslcert cert.pem &&  npm run startServer

