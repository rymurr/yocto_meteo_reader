### Library for reading the sensors off a yoctopuce
temperature, humidity, pressure sensor USB stick

The library publishes the data onto a zmq pub bus
and its sent out to various interested parties (storage, live plotting etc)

##TODO:

* make code safe and 'deploy' it
* write some tests and docs
* lots of error checking!
* finish adding storage message type, need to integrate into subscriber and writer and force conversion
* compile flags around Mongo stuff (and protobuf?). Test mongo, do protobuf
* auto generate help string for msg types/file types based on available libraries
* add yocto lib as a submodule

##DEPLOY:

* build on arm
* always running on rpi
* rotating data removal on rpi
* shipping to server/mongo db
* script to recover data on rpi in case server is down
* should always be running on server

