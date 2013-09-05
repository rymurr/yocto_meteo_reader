### Library for reading the sensors off a yoctopuce
temperature, humidity, pressure sensor USB stick

The library publishes the data onto a zmq pub bus
and its sent out to various interested parties (storage, live plotting etc)

##STATUS:
Works on x86 w/o a problem for all message types and intputs/outputs. Currently some problems on arm:

* cant build bson message type
* extremely limited compile time resources: long compilation time and poor logging configuration


##TODO:

* make code safe and 'deploy' it
* write some tests and docs
* lots of error checking!
* finish refactor of subscriber and build on arm
* build on electron

##DEPLOY:

* always running on rpi
* rotating data removal on rpi
* shipping to server/mongo db
* script to recover data on rpi in case server is down
* should always be running on server

