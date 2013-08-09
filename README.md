### Library for reading the sensors off a yoctopuce
temperature, humidity, pressure sensor USB stick

The library publishes the data onto a zmq pub bus
and its sent out to various interested parties (storage, live plotting etc)

##TODO:

* add some command line options: list of sensors, which boxes, publish port
* add cli for recievers: message types, storage options
* add storage layer to subscriber
* should I look into switching to BSON? Would fit better with mongodb storage layer?
* go lower down w/ zmq so I can properly handle binary data
* fix SensorGroup and Sensor class. Static bits are really annoying. Need to fix yocto lib for that
* once static bits are fixed need to refactor for better generality and separation of duties


#BSON:

* add bson library to cmake
* convert protobuf code to bson code in sensor.cpp
* need to replace meteo::SensorReading with BSONObj in a lot of places
* convert zmq send method to put binary form of BSON onto wire
* convert zmq recieve method to read in a BSON obj directly from the wire
* recieve bson directly instead of converting from string
* remove protobuf from cmake/project


#STORAGE:

* queue to store incoming messages and dump to disk whenever it gets 'large'
* should probably append to a file
* 1 file per day
* have to iterate over files periodically to remove stale data periodically
* define periodicity and removal
* option to connect to mongo and dump directly there
