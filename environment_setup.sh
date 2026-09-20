#!/bin/sh
sudo insmod mostima.ko
sudo ip link set mostima0 up
sudo ip addr add 10.0.0.1/24 dev mostima0
