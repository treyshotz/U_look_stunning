FROM node

WORKDIR /usr/src/app

COPY ["package.json", "package-lock.json*", "./"]

COPY . .

RUN npm install --save express  &&  npm install -g peer  && npm install -g nodemon


EXPOSE 3000

CMD npm run startServer

FROM node

WORKDIR /usr/src/app

COPY ["package.json", "package-lock.json*", "./"]

COPY . .

RUN npm install &&  npm install -g peer

CMD  cd cert && peerjs --port 9000 --sslkey key.pem --sslcert cert.pem