LOCAL_PATH:= $(call my-dir)

# ========================================================
# nano
# ========================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= mbtest.c

LOCAL_SRC_FILES += ../src/mblen.c

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)

LOCAL_MODULE := mbtest
LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)
LOCAL_MODULE_TAGS := debug
include $(BUILD_EXECUTABLE)

