#There are 6 DEBUG values we should have:
#TRACE
#DEBUG
#INFO
#WARN
#ERROR
#FATAL
#IF DEBUG_CMD HAS NO VALUE THEN IT IS A RELEASE BUILD (NON DEBUG)
DEBUG_CMD=DEBUG
ifdef DEBUG_CMD
ifeq ($(DEBUG_CMD),DEBUG)
#DONT FORGET THE SPACE AFTER ifeq https://stackoverflow.com/a/16770163
	DEBUG_FLAGS=-DDEBUG
#DONT FORGET TO TAB VARIABLES https://stackoverflow.com/a/16931877
endif
# ifeq($(DEBUG_CMD),TRACE)
# DEBUG_FLAGS=
# endif
# ifeq($(DEBUG_CMD),INFO)
# DEBUG_FLAGS=
# endif
# ifeq($(DEBUG_CMD),WARN)
# DEBUG_FLAGS=
# endif
# ifeq($(DEBUG_CMD),ERROR)
# DEBUG_FLAGS=
# endif
# ifeq($(DEBUG_CMD),FATAL)
# DEBUG_FLAGS=
# endif
endif
#sources
#https://stackoverflow.com/questions/38777722/makefile-error-out-if-variable-not-set
#http://www.people.vcu.edu/~jsiebers/mcnpinfo/dcomment/manual/node9.html
#how to make errors on the makefile level!
#https://stackoverflow.com/questions/1864385/how-to-force-an-error-in-a-gnumake-file
#https://www.rapidtables.com/code/linux/gcc/gcc-d.html
