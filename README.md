### Library for reading the sensors off a yoctopuce
temperature, humidity, pressure sensor USB stick

The library publishes the data onto a zmq pub bus
and its sent out to various interested parties (storage, live plotting etc)

##TODO:

* add some command line options: list of sensors, which boxes, publish port
* add cli options for recievers: message types, storage options
* make code safe and 'deploy' it
* write some tests and docs
* add logging!
* lots of error checking!
* check thread safety and queue clearing in writers
* memory leak testing 
* add zero padding to filenames
* increase number of values before a commit to file/mongo
* write json storage message type

##DEPLOY:

* build on arm
* always running on rpi
* rotating data removal on rpi
* shipping to server/mongo db
* script to recover data on rpi in case server is down
* should always be running on server
* get wifi working on rpi
