ip link set can0 down
ip link set can0 type can bitrate 500000
ip -details link show can0
ip link set can0 up
candump can0
#./test | head -n 10 | grep 'get = '
#./test | head -n 10 | sort
