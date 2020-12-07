# TRACEROUTE

Traceroute program written in C that displays the route and measures transit delays of packets to the target IP. This implementation sends ICMP *echo requests* instead of UDP packets. Similar to ```traceroute -I```.

## Run

```sudo ./traceroute <ip-address>```

## Usage

* ```make``` - compile
* ```make clean``` - remove temporary objects
* ```make distclean``` - remove everything besides source files
