PROJECT_DIR=$(pwd)

AR_SOCKET=/tmp/ar.sock
VIZ_SOCKET=/tmp/viz.sock
ESP32_PORT=/dev/rfcomm0
DRONE_STL_FILE=$PROJECT_DIR/resource/ardrone.stl

unlink $AR_SOCKET
unlink $VIZ_SOCKET

if [ -f $ESP32_PORT ]; then
    echo connect to port $ESP32_PORT
else
    exit -1
fi


mkdir -p log
TS=$(date "+%Y%m%d-%H%M%S")

./build/ar --sock=$AR_SOCKET -  > log/$TS.txt &
./build/visualizer --sock=$VIZ_SOCKET > log/$TS.txt &
./build/main --esp32=$ESP32_PORT --sock-viz=$AR_SOCKET --sock-ar=$VIZ_SOCKET > log/$TS.txt &
