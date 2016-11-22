#!/bin/bash

# Remove the driver
sudo rmmod vlc.ko

# Insert the driver
sudo insmod vlc.ko frq=6 pool_size=5 tx_len=1 mtu=1300 broadcast=1 mac_or_app=1 self_id=8 dst_id=9 show_msg=0

# Configure the IP address of the new interface
sudo ifconfig vlc0 192.168.0.1

# Specify the TX: 0 is low-power LED; 1 is high-power LED
sudo echo 0 > /proc/vlc/tx

# Specify the RX: 0 is low-power LED; 1 is PD
sudo echo 1 > /proc/vlc/rx
