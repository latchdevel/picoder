# picoder

Simple standalone command line tool to decode and encode OOK protocols supported by [**"pilight"**](https://github.com/pilight/pilight) project.

Works on any libc/libc++ compatible system, like MacOS, FreeBSD, Linux, even Windows.

[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
[![Build tests](https://github.com/latchdevel/picoder/actions/workflows/BuildTest.yml/badge.svg)](https://github.com/latchdevel/picoder/actions/workflows/BuildTest.yml)

Based on [**PiCode library**](http://github.com/latchdevel/PiCode) that provide a standard C/C++ library to should help to implement IoT bridges between the RF 433/315MHz band and internet protocols.

A large number of wireless protocols of RC switches and weather stations using the 433/315Mhz radio frequency band via ASK/OOK pulse modulation are implemented by the excellent work of the "pilight" community. A list of supported protocols can be found in the pilight manual: https://manual.pilight.org/protocols/433.92/index.html

## BUILD
 ```
$ git clone --recursive https://github.com/latchdevel/picoder 
$ cd picoder
$ mkdir build
$ cd build
$ cmake .. (or "cmake -DCMAKE_BUILD_TYPE=debug .." for debug)
$ make
$ make install (optional)
$ make uninstall (to uninstall)
```

## USAGE
```
picoder <command> [options]
   commands:
       help | -h | --help                           --> show help
       list [-h] [-e] [-d devtype] [-d devtype] ... --> list supported protocols and devices
            [-h | --help]                           --> show command options
            [-e | --encode]                         --> list encode support only protocols
            [-d | --device devtype ]                --> list device type only protocols
       show [-h] -p protocol                        --> show protocol details
            [-h | --help]                           --> show command options
            [-p | --proto protocol]                 --> set protocol to show
       encode [-h] -p protocol -j json [-r repeats] --> encode from protocol and json data
              [-h | --help]                         --> show command options
              [-p | --proto protocol]               --> set protocol to encode
              [-j | --json json-data]               --> set json data to encode
              [-f | --full-json json]               --> set full json to encode
              [-r | --repeats repeats]              --> add repeats parameter from 1 to 32
              [-t | --train]                        --> show pulse train
              [-o | --only-train]                   --> show only pulse train
       decode [-h] [ -s string | -t train ]         --> decode pilight string or pulse train
              [-h | --help]                         --> show command options
              [-s | --string piligth-string]        --> pilight string to decode
              [-t | --train pulse-train]            --> pulse train to decode
       convert [-h] [ -s string | -t train ]        --> coverts from/to pilight string to/from pulse train
               [-h | --help]                        --> show command options
               [-s | --string piligth-string]       --> pilight string to convert
               [-t | --train pulse-train]           --> pulse train to convert
       version | -v | --version                     --> show version details
```

## EXAMPLES

### Encode to pilight string:
* From protocol and json data:
  ```
  $ picoder encode -p arctech_switch -j '{"id":92,"unit":0,"on":1}'

  c:010002000200020002000200020002000200020002000200020002000200020002000200020002020000020200020002000002000200020200000200020002000203;p:315,2835,1260,10710@
  ```
* From full json:
  ```
  $ picoder encode -f '{ "conrad_rsl_switch" : {"id":1,"unit":2,"on":1} }' -r 5

  c:011010100101011010100110101001100110010101100110101010101010101012;p:1400,600,6800;r:5@
  ```

### Decode from pilight string:
```
$ picoder decode -s "c:011010100101011010100110101001100110010101100110101010101010101012;p:1400,600,6800@"

[{
  "conrad_rsl_switch": {
    "id": 1,
    "unit": 2,
    "state": "on"
  }
}]
```

### Convert from pilight string to pulse train:
```
$ picoder convert -s "c:001010101100101010101010101010110010101102;p:700,1400,7650@"

pulses[42]={700,700,1400,700,1400,700,1400,700,1400,1400,700,700,1400,700,1400,700,1400,700,1400,700,1400,700,1400,700,1400,700,1400,700,1400,700,1400,1400,700,700,1400,700,1400,700,1400,1400,700,7650};
```

### Convert from pulse train to pilight string:
```
$ picoder convert -t "325,768,2438,2082,1052,1632,94,111,82,882,332,595,81,87,275,913,1638,555,158,602,138,116,234,170,852,145,163,475,89,294,127,82,99,118,104,82,577,160,131,93,542,82,99,120,112,91,125,103,86,81,89,130,93,96,116,91,316,82,82,172,103,101,281,102,90,113,161,84,171,158,204,335,89,82,185,237,127,473,98,82,1090,845,357,81,515,143,198,8946"

c:0123456661067707568688081886808888886888688888888888888808888808888888008880868841086889;p:325,768,2438,2082,1052,1632,595,913,158,8946@
```


### Show protocol list:
```
$ picoder list

Encode Protocol             Type      Devices
-----------------------------------------------------------------------------------
 [ ]   alecto_ws1700        weather   iBoutique Weather Stations
                                      Alecto WS1700 Weather Stations
 [ ]   alecto_wsd17         weather   Alecto WSD-17 Weather Stations
 [ ]   alecto_wx500         weather   Balance RF-WS105 Weather Stations
                                      Meteoscan W1XXX Weather Stations
                                      Hama EWS1500 Weather Stations
                                      Ventus WSXXX Weather Stations
                                      Auriol H13726 Weather Stations
                                      Alecto WX500 Weather Stations
 [ ]   arctech_contact      contact   D-IO Contact Sensor
                                      KlikAanKlikUit Contact Sensor
 [*]   arctech_dimmer       dimmer    KlikAanKlikUit Dimmers
 [ ]   arctech_dusk         dusk      KlikAanKlikUit Dusk Sensor
 [ ]   arctech_motion       motion    KlikAanKlikUit Motion Sensor
 [*]   arctech_screen       screen    DI-O Screens
                                      KlikAanKlikUit Screens
 [*]   arctech_screen_old   screen    Old KlikAanKlikUit Screens
 [*]   arctech_switch       switch    Intertechno Switches
                                      CoCo Technologies Switches
                                      Nexa Switches
                                      D-IO Switches
                                      KlikAanKlikUit Switches
 [*]   arctech_switch_old   switch    Düwi Terminal Switches
                                      Bye Bye Standby Switches
                                      Old Intertechno Switches
                                      Cogex Switches
                                      Old KlikAanKlikUit Switches
 [ ]   auriol               weather   Auriol Weather Stations
 [*]   beamish_switch       switch    beamish_switch Switches
 [*]   clarus_switch        switch    Clarus Switches
 [*]   cleverwatts          switch    Cleverwatts Switches
 [ ]   conrad_rsl_contact   contact   Conrad RSL Contact Sensor
 [*]   conrad_rsl_switch    switch    Conrad RSL Switches
 [*]   daycom               switch    Daycom switches
 [*]   ehome                switch    eHome Switches
 [*]   elro_300_switch      switch    Elro 300 Series Switches
 [*]   elro_400_switch      switch    Elro 400 Series Switches
 [ ]   elro_800_contact     contact   Elro Series 800 Contact
 [*]   elro_800_switch      switch    Maxi-Tronic FUNK-LIGHT switches
                                      Brennenstuhl Comfort
                                      Elro 800 series Switches
 [*]   eurodomest_switch    switch    Eurodomest Switches
 [ ]   ev1527               contact   ev1527 contact sensor
 [ ]   fanju                weather   Fanju 3378 Weather Stations
 [*]   heitech              switch    Heitech series Switches
 [*]   impuls               switch    Impuls Switches
 [ ]   iwds07               contact   Golden Security iwds07 contacts
 [ ]   kerui_D026           contact   KERUI D026 Door sensor
 [*]   logilink_switch      switch    Logilink Switches
 [*]   mumbi                switch    Mumbi Switches
 [ ]   nexus                weather   Sencor SWS 21TS Weather Stations
                                      Digoo DG-R8H/DG-R8S Weather Stations
                                      Nexus Weather Stations
 [ ]   ninjablocks_weather  weather   Ninjablocks Weather Sensors
 [*]   pollin               switch    Pollin Switches
 [*]   quigg_gt1000         switch    Quigg GT-1000 protocol
 [*]   quigg_gt7000         switch    Quigg GT-7000 Switches
 [*]   quigg_gt9000         switch    Quigg GT-9000 remote with GT-FSi-06 switches
 [*]   quigg_screen         screen    Quigg Switch Screen
 [*]   rc101                switch    rc102 Switches
                                      rc101 Switches
 [*]   rev1_switch          switch    Rev Switches v1
 [*]   rev2_switch          switch    Rev Switches v2
 [*]   rev3_switch          switch    Rev Switches v3
 [*]   rsl366               switch    Pro MAX Switches
                                      RSL366 Switches
 [ ]   sc2262               contact   sc2262 contact sensor
 [ ]   secudo_smoke_sensor  alarm     Secudo/FlammEx smoke sensor
 [*]   selectremote         switch    SelectRemote Switches
 [*]   silvercrest          switch    Silvercrest Switches
 [*]   smartwares_switch    switch    Smartwares Switches
 [ ]   tcm                  weather   TCM 218943 weather stations
 [*]   techlico_switch      switch    TechLiCo Lamp
 [ ]   teknihall            weather   Teknihall Weather Stations
 [ ]   tfa2017              weather   TFA 30.X Temp Hum Sensor Revision 09/2017
 [ ]   tfa30                weather   TFA 30.X Temp Hum Sensor
 [ ]   tfa                  weather   Globaltronics GT-WT-01 Weather Station
                                      Freetec NC7104-675 Weather Station
                                      SOENS Weather Stations
                                      Conrad Weather Stations
                                      TFA weather stations
 [*]   x10                  switch    x10 based devices
```

### Show protocol details:
```
$ picoder show -p mumbi

Protocol:    mumbi
Encode:      Supported
Device type: 1 (switch)
Devices:     Mumbi Switches
MinRawLen:      50 uSecs
MaxRawLen:      50 uSecs
MinGapLen:   10438 uSecs
MaxGapLen:   10778 uSecs
Option: (Id) Name          Arg Conf Regexp mask
         (0) confirm        2   6   ^[10]{1}$
         (0) confirm        2   6   ^[10]{1}$
         (f) off            1   2
         (t) on             1   2
         (u) unitcode       2   1   ^(3[012]?|[012][0-9]|[0-9]{1})$
         (s) systemcode     2   1   ^(3[012]?|[012][0-9]|[0-9]{1})$
pilight-send:
	 -s --systemcode=systemcode	control a device with this systemcode
	 -u --unitcode=unitcode		control a device with this unitcode
	 -t --on			send an on signal
	 -f --off			send an off signal
```

## To do:
- [ ] Support for [value change dump](https://en.wikipedia.org/wiki/Value_change_dump) "vcd" output format (to Logical/Wave Analyzers)
- [ ] Support for [pilight-send](https://manual.pilight.org/programs/send.html) input format (pilight-send -p quigg_gt7000 -i 2816 -u 1 -f)

# License
Copyright (c) 2021-2022 Jorge Rivera. All right reserved.

License GNU Lesser General Public License v3.0.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

See the [LICENSE](LICENSE.md) file for license rights and limitations (lgpl-3.0).
