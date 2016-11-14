#!/bin/bash

rmmod vlc.ko

# Insert the driver
insmod vlc.ko frq=6 pool_size=5 tx_len=2 mtu=1300 mac_or_app=0 self_id=8 dst_id=9 show_msg=1

# Configure the IP address of the new interface
ifconfig vlc0 192.168.0.1

# Specify the TX: 0 is low-power LED; 1 is high-power LED
echo 1 > /proc/vlc/tx

# Specify the RX: 0 is low-power LED; 1 is PD
echo 1 > /proc/vlc/rx
