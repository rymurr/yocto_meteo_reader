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


#STORAGE:

* queue to store incoming messages and dump to disk whenever it gets 'large'
* should probably append to a file
* 1 file per day
* have to iterate over files periodically to remove stale data periodically
* define periodicity and removal
* option to connect to mongo and dump directly there
