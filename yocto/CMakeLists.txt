cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

include_directories (.)

set(c_yocto_src yapi/yapi.c  yapi/yfifo.c  yapi/yhash.c  yapi/yjson.c yapi/ykey.c  yapi/ymemory.c  yapi/ypkt_lin.c  yapi/ypkt_osx.c  yapi/ypkt_win.c  yapi/yprog.c  yapi/ystream.c  yapi/ytcp.c  yapi/ythread.c)
set(cpp_yocto_src yocto_anbutton.cpp       yocto_colorled.cpp    yocto_display.cpp    yocto_hubport.cpp   yocto_lightsensor.cpp  yocto_pressure.cpp  yocto_temperature.cpp  yocto_vsource.cpp yocto_api.cpp            yocto_current.cpp     yocto_dualpower.cpp  yocto_humidity.cpp  yocto_network.cpp      yocto_relay.cpp     yocto_voc.cpp          yocto_watchdog.cpp yocto_carbondioxide.cpp  yocto_datalogger.cpp  yocto_files.cpp      yocto_led.cpp       yocto_power.cpp        yocto_servo.cpp     yocto_voltage.cpp      yocto_wireless.cpp)
add_library(yocto SHARED ${c_yocto_src} ${cpp_yocto_src})
