TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

PROD_HOST += caExample
caExample_SRCS	+= caExample.c
caExample_LIBS	+= $(EPICS_BASE_HOST_LIBS)

PROD_HOST += caMonitor
caMonitor_SRCS	+= caMonitor.c
caMonitor_LIBS	+= $(EPICS_BASE_HOST_LIBS)

PROD_HOST += ex_1
ex_1_SRCS	+= ex_1.c
ex_1_LIBS	+= $(EPICS_BASE_HOST_LIBS)

PROD_HOST += ex_2
ex_2_SRCS	+= ex_2.c
ex_2_LIBS	+= $(EPICS_BASE_HOST_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

