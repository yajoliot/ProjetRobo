#!/bin/sh
source variables
echo $CURRENT_DIR
if [ $1 == "release" ]
	then
		mv $CURRENT_DIR/variables.mk variables.mk.old && awk '{gsub(/DEBUG_CMD=.*/,"DEBUG_CMD=")}1' variables.mk.old > $CURRENT_DIR/variables.mk && rm variables.mk.old
elif [ $1 == "debug" ]
	then
		mv $CURRENT_DIR/variables.mk variables.mk.old && awk '{gsub(/DEBUG_CMD=.*/,"DEBUG_CMD=DEBUG")}1' variables.mk.old > $CURRENT_DIR/variables.mk && rm variables.mk.old
else
	echo "Unknown parameter"
fi

#sources:
#https://www.macs.hw.ac.uk/~hwloidl/Courses/LinuxIntro/x945.html
#http://linuxcommand.org/lc3_wss0120.php
#https://askubuntu.com/questions/20414/find-and-replace-text-within-a-file-using-commands
