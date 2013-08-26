### Library for reading the sensors off a yoctopuce
temperature, humidity, pressure sensor USB stick

The library publishes the data onto a zmq pub bus
and its sent out to various interested parties (storage, live plotting etc)

##TODO:

* make code safe and 'deploy' it
* write some tests and docs
* lots of error checking!
* write json,protobuf storage message type and their conversions
* make message stuff more flexible 
* add cli argument for message out type

##DEPLOY:

* build on arm
* always running on rpi
* rotating data removal on rpi
* shipping to server/mongo db
* script to recover data on rpi in case server is down
* should always be running on server
* mongo doesn't compile on arm...have to fix up message types
