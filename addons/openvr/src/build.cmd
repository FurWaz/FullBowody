cls
g++ ./main.cpp -shared -o ../plugin/bin/win64/driver_FullBowody.dll -I../../../include/ -L../../../lib/ -lopenvr_api -lws2_32
echo build finished