#!/bin/bash

FILE=$1/platform.h

if [ -f $FILE ]; then
	[ -d $2 ] || mkdir -p $2
	cp -f $1/platform.h $2/platform.h
fi
