#!/bin/bash
sudo ifconfig enx0000120458b7  192.168.68.1/24
ssh root@192.168.68.2
mount -t nfs -o nolock,vers=3 192.168.68.1:/home/ssk/Desktop/linux/driver /mnt
insmod *.ko
rmmod *.ko
target remote 192.168.68.2:12345