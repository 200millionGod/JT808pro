include ../../Makefile.param

TARGET:=~/jt808_app

INC += -I./ -I$(TARGET_DIR)/../../$(TARGET_INC_DIR)  -I../interface
LIB += -L$(TARGET_DIR)/../../$(TARGET_LIB_DIR) -lapp -ljt808 -lsocket -lbasecom -pthread -ldl 

#SRCS := $(wildcard *.cpp)
SRCS += main.cpp 
OBJS := $(patsubst %.cpp,%.o, $(SRCS))

.PHONY : clean all

all: $(OBJS) $(TARGET)

$(OBJS):%.o:%.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $^ -o $@

$(TARGET): $(OBJS)
	$(CXX) $^ $(CXXFLAGS) $(INC) $(LIB) -o $@

clean:
	rm -rf *.o
	rm -rf $(TARGET)
