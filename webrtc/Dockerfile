FROM node:latest

WORKDIR /usr/src/app

COPY ["package.json", "package-lock.json*", "./"]

COPY . .

EXPOSE 443

RUN npm install --save express  && npm install -g nodemon


CMD   npm run startServer

