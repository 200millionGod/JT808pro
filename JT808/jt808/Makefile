TOPDIR = $(shell pwd)

SRC = $(TOPDIR)/src

.PHONY: clean all

all:bin

bin:
	./src/ver.sh
	$(MAKE) -C $(SRC)

	
clean:
	$(MAKE) -C $(SRC) clean	
