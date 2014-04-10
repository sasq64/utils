ifeq ($(GRAPPIX_INCLUDED),)
GRAPPIX_INCLUDED = 1
GRAPPIX_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

CPP_MODS := $(realpath $(CPP_MODS))
ifeq ($(CPP_MODS),)
  CPP_MODS := $(realpath ../cpp-mods)
endif

ifeq ($(CPP_MODS),)
  $(error Can not find 'cpp-mods'. Please set CPP_MODS to point to that directory)
endif

INCLUDES += $(GRAPPIX_DIR) $(GRAPPIX_DIR)grappix/freetype-gl
DIRS += $(GRAPPIX_DIR)grappix $(GRAPPIX_DIR)grappix/freetype-gl $(GRAPPIX_DIR)grappix/shaders
CFLAGS +=  -DWITH_FREETYPE

ifeq ($(HOST),android)
  DIRS += $(GRAPPIX_DIR)grappix/android
  LIBS += -lz -llog -landroid -lEGL -lGLESv2
else ifeq ($(HOST),emscripten)
  DIRS += $(GRAPPIX_DIR)grappix/pc
else ifeq ($(HOST),apple)
  DIRS += $(GRAPPIX_DIR)grappix/pc
  LIBS += -lglfw -framework OpenGL -lGLEW
else
  DIRS += $(GRAPPIX_DIR)grappix/pc
  LIBS += -lglfw -lGL -lGLEW
endif

include $(CPP_MODS)/image/module.mk
include $(GRAPPIX_DIR)freetype/module.mk
include $(CPP_MODS)/coreutils/module.mk

endif