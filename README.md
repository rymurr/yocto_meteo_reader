### Library for reading the sensors off a yoctopuce
temperature, humidity, pressure sensor USB stick

The library publishes the data onto a zmq pub bus
and its sent out to various interested parties (storage, live plotting etc)

##TODO:

* add some command line options: list of sensors, which boxes, publish port
* add cli for recievers: message types, storage options
* add storage layer to subscriber
* fix SensorGroup and Sensor class. Static bits are really annoying. Need to fix yocto lib for that
* once static bits are fixed need to refactor for better generality and separation of duties
* code needs some serious cleaning and refactoring
* make code safe and 'deploy' it
* write some tests


##STORAGE:

* have to iterate over files periodically to remove stale data periodically
* define periodicity and removal

##DEPLOY:

* build on arm
* always running on rpi
* rotating data removal on rpi
* shipping to server/mongo db
* script to recover data on rpi in case server is down
* should always be running on server
* get wifi working on rpi
