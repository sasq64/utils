ifeq ($(SQLITE3_INCLUDED),)
SQLITE3_INCLUDED = 1

THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
LIBS += -ldl
INCLUDES += $(THIS_DIR)..
sqlite3_FILES := $(THIS_DIR)database.cpp $(THIS_DIR)sqlite3.c

MODULES += sqlite3

endif