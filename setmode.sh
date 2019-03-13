#!/bin/sh
source variables
if [ $1 == "release" ]
	then
		awk '{gsub(/DEBUG_CMD=.*/,"DEBUG_CMD=")}' $CURRENT_DIR/variables.mk > $CURRENT_DIR/variables.mk
elif [ $1 == "debug" ]
	then 
		awk '{gsub(/DEBUG_CMD=.*/,"DEBUG_CMD=DEBUG")}' $CURRENT_DIR/variables.mk > $CURRENT_DIR/variables.mk
else
	echo "Unknown parameter"