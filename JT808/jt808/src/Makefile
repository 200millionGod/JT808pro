SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o, $(SRCS))
PWD = $(shell pwd)

include ../config.mk
 
CFLAGS  = $(BIN_CFLAGS)
CPPFLAGS = $(BIN_CFLAGS)  -std=c++11
LDFLAGS = $(BIN_LDFLAGS)

#%.o:%.cpp
#	$(CXX) -c $(SRC) $(CPPFLAGS)

bin:$(OBJS)
	$(CXX) -o $(BIN_TARGET) $(OBJS) $(CPPFLAGS) $(LDFLAGS)
	if [ ! -d $(TOPDIR)/../../$(TARGET_BIN_DIR) ]; then \
		mkdir -p $(TOPDIR)/../../$(TARGET_BIN_DIR); \
	fi
	$(CP) $(BIN_TARGET) $(TOPDIR)/../../$(TARGET_BIN_DIR)/ -rf
	
%.o:%.cpp  
#	$(CXX) $(CPPFLAGS) $(SRCS) -c $@
#	$(CXX) $(CXXFLAGS) $(INC) -c $^ -o $@
	$(CXX) -c $(CPPFLAGS) $< -o $@ 		

clean:
	-@rm *.o
	-@rm $(BIN_TARGET)
