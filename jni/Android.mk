# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := android-segv
LOCAL_SRC_FILES := android-segv.c

LOCAL_LDLIBS    := -llog

LOCAL_CFLAGS += -fverbose-asm
LOCAL_CFLAGS += -v
ifeq (,$(findstring clang,$(NDK_TOOLCHAIN_VERSION)))
LOCAL_FILTER_ASM := python -c 'import sys; import shutil; src = open(sys.argv[1], "rb"); dst = open(sys.argv[2], "wb"); shutil.copyfileobj(src,dst);'
endif

LOCAL_LDFLAGS += -Wl,-Map,$$(LOCAL_OBJS_DIR:%/=%)/$(LOCAL_MODULE).map -Wl,--cref -Wl,--demangle

include $(BUILD_SHARED_LIBRARY)
