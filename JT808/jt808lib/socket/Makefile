include ../../Makefile.param

TARGET:=libsocket.a
INC += -I./  

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o, $(SRCS))

.PHONY : clean all

all: $(OBJS) $(TARGET)

$(OBJS):%.o:%.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $^ -o $@

$(TARGET):
	$(AR) -rcs $(TARGET) $(OBJS)
	$(CP) $(TARGET) $(TARGET_DIR)/../../$(TARGET_LIB_DIR)/ -rf
	$(CP) *.h $(TARGET_DIR)/../../$(TARGET_INC_DIR)/ -rf

clean:
	rm -rf *.o
	rm -rf $(TARGET)