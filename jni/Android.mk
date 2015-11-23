LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CXXFLAGS += -std=c++11 -O2

LOCAL_MODULE    := SimpleEngine
LOCAL_CFLAGS    := -Wall -Wextra
LOCAL_LDLIBS 	:= -lGLESv2 -L$(SYSROOT)/usr/lib -llog

#traverse all the directory and subdirectory
define walk
  $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

#find all the file recursively under jni/
ALLFILES = $(call walk, $(LOCAL_PATH))
FILE_LIST := $(filter %.cpp, $(ALLFILES))

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/base/utils 		\
					$(LOCAL_PATH)/base 				\
					$(LOCAL_PATH)/base/shaders 		\
					$(LOCAL_PATH)/base/node			\
					$(LOCAL_PATH)/base/node/light	\
					$(LOCAL_PATH)/base/texture		\
					$(LOCAL_PATH)/base/geometry		\
					$(LOCAL_PATH)/base/font			\
					$(LOCAL_PATH)/game

include $(BUILD_SHARED_LIBRARY)