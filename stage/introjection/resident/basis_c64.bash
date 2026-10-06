#!/bin/bash


function compile()
{
	
	cd %CA2_ROOT%platform/seed/metaseed


g++ -fpermissive -fexceptions -fnon-call-exceptions -fno-pie -fno-stack-protector -Wno-pointer-arith -Wno-attributes -Winvalid-pch -fPIC -std=c++17  -g -D_DEBUG   -I/xcore/app-core/appseed/resident -I../../../include -I../../../app/thirdparty/appseed -I../../../inc -I../../../include -I../../../app/include -I../../../platform/include -I. -I.. -I../.. -I../aura -I../../platform/haiku -I../../platform/haiku/include -I../../../app -I../../../app/include -I../../.. -I../../../app/appseed -I../../../app/appseed/aura -I../../../app/appseed/base -I../../../app/appseed/axis -I../../../app/appseed/aura -I../../../app/axis   -c "%SOURCE%" -o "%ITEM_NAME%.o"


}


compile
