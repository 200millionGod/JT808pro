include ../../Makefile.param
BIN_TARGET = ~/test/jtt808

TOPDIR=$(PWD)
LIB_DIR = $(TOPDIR)/../lib
LD_PATH = $(LIB_DIR)
LD_THIRD_PATH = $(TOPDIR)/../third_party

#BIN_CFLAGS   = -g -Wall -Werror  
BIN_CFLAGS   = -g -Wall -lstdc++ -pthread -lsupc++ -lstlport_shared -lVideoClient -D__ANDROID_API__=17
BIN_CFLAGS  += -I$(TOPDIR)/../inc\
			   -I$(TOPDIR)/../../$(TARGET_INC_DIR)\
			   -I"$(TOPDOR)/../third_party/libjsoncpp/inc"\
			   -I"$(TOPDIR)/../third_party/aip-cpp-sdk-0.7.4"\
#			   -I"$(DYNAMIC_LIB_PATH)/libiconv-1.16/build/include"
#			   -I"$(DYNAMIC_LIB_PATH)/boost/include"\
#			   -I"$(DYNAMIC_LIB_PATH)/boost_1_66_0"
#			   -I"$(DYNAMIC_LIB_PATH)/usr/include" \
#			   -I"$(DYNAMIC_LIB_PATH)/usr/include/amba"\
#			   -I"$(DYNAMIC_LIB_PATH)/usr/include/amba/$(BUILD_ARCH)" \
#			   -I"$(DYNAMIC_LIB_PATH)/usr/include/glib-2.0" \
#		 	   -I"$(DYNAMIC_LIB_PATH)/usr/include/gio-unix-2.0"
				
BIN_LDFLAGS += -lcurl -lapp -ljt808 -lsocket -lbasecom -lz -ljsoncpp -ldl -l:libiconv.a -llog -lm -l:libgpsinfo.a -l:librilinfo.a -l:libspnl.a
#BIN_LDFLAGS += -lcurl -lapp -ljt808 -lsocket -lbasecom -lz -ljsoncpp -ldl -lssl -lssh2 -liconv

#BIN_LDFALGS += -lcurl -lapp -ljt808 -lsocket -liconv -lssl -lcrypto -lz -lssh2 -ljsoncpp -ldl

BIN_LDFLAGS += -L$(LD_PATH)\
			   -L/usr/local/arm/ndk-toolchain/arm-linux-androideabi/sysroot/usr/lib\
			   -L/usr/local/arm/android-ndk-r14b/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi\
			   -L$(LD_THIRD_PATH)/libjsoncpp/lib\
			   -L$(TOPDIR)/../../$(TARGET_LIB_DIR)\
			   -L$(TOPDIR)/../third_party/libjsoncpp/lib\
			   -L$(DYNAMIC_LIB_PATH)/lib\
			   -L$(TOPDIR)/../third_party/lib\
#			   -L/usr/lib\
#			   -L$(DYNAMIC_LIB_PATH)/boost/lib\
#			   -L$(DYNAMIC_LIB_PATH)/boost_1_66_0/android/lib\
#			   -L$(DYNAMIC_LIB_PATH)/usr/lib/pulseaudio \
#			   -L$(DYNAMIC_LIB_PATH)/usr/local/lib \
#			   -L$(DYNAMIC_LIB_PATH)/usr/lib \
			   -L$(DYNAMIC_LIB_PATH)/usr/local/lib \
			   -L$(DYNAMIC_LIB_PATH)/usr/lib/pulseaudio \
			   -Wl,-rpath=/usr/local/lib\
			   -Wl,-rpath=/usr/lib/pulseaudio\
#			   -Wl,-rpath=$(DYNAMIC_LIB_PATH)/lib\
#			   -Wl,-rpath=$(DYNAMIC_LIB_PATH)/usr/lib

