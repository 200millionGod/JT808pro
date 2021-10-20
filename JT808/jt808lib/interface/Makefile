include ../../Makefile.param
#include ../../jt808/config.mk
BIN_CFLAGS=../third_party/include
#CPPFLAGS = $(BIN_CFLAGS)  -std=c++11
LD_PATH=../third_party/lib
BIN_LDFLAGS+=-L$(LD_PATH)

TARGET:=libapp.a
#INC += -I./ -I../common -I../public/basecom -I../socket -I../jt808 $(BIN_CFLAGS)
INC += -I./ -I../common -I../public/basecom -I../socket -I../jt808 -I$(BIN_CFLAGS)

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o, $(SRCS))

.PHONY : clean all

all: $(OBJS) $(TARGET)

$(OBJS):%.o:%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) $(BIN_LDFLAGS) -c $^ -o $@

$(TARGET):
	$(AR) -rcs $(TARGET) $(OBJS)
	$(CP) $(TARGET) $(TARGET_DIR)/../../$(TARGET_LIB_DIR)/ -rf
	$(CP) *.h $(TARGET_DIR)/../../$(TARGET_INC_DIR)/ -rf

clean:
	rm -rf *.o
	rm -rf $(TARGET)
