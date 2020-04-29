#
# This is a generic makefile for compiling uOS modules.
# It is called with the following parameters:
# OS - absolute path of uOS sources
# TARGET - absolute path of target compilation directory
# MODULE - name of the compiled module
# DRIVERS - absolute path of uOS drivers
ARFLAGS = r
include $(TARGET)/target.cfg

# Find the module sources in uos or in contrib directory.

ifeq ($(OS)/$(MODULE), $(wildcard $(OS)/$(MODULE)))
    MODULEDIR = $(OS)/$(MODULE)
else
	ifeq ($(DRIVERS)/../$(MODULE), $(wildcard $(DRIVERS)/../$(MODULE)))
   		MODULEDIR = $(DRIVERS)/../$(MODULE)
   	else
    	MODULEDIR = $(TARGET)/$(MODULE)
   	endif	
endif

# Get module parameters
include $(MODULEDIR)/module.cfg

clean::
		rm -rf *.[oai] *~ .deps

include $(OS)/rules.mak
###
