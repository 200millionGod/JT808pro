include ../Makefile.param

.PHONY : create_dir cp_common clean lib demo all
all: create_dir cp_common
	cd ./socket; make; cd -
	cd ./public/basecom/; make; cd -
	cd ./jt808; make; cd -
	cd ./interface; make; cd -
	cd ./main; make; cd -

demo:
	cd ./main; make; cd -

lib: cp_common
	cd ./socket; make; cd -
	cd ./public/basecom/; make; cd -
	cd ./jt808; make; cd -
	cd ./interface; make; cd -
	
clean:
	rm -rf $(TARGET_DIR)/../$(TARGET_INC_DIR)
	rm -rf $(TARGET_DIR)/../$(TARGET_LIB_DIR)
	cd ./socket; make clean; cd -
	cd ./public/basecom/; make clean; cd -
	cd ./jt808; make clean; cd -
	cd ./interface; make clean; cd -
	cd ./main; make clean; cd -

cp_common:
	cp ./common/*.h $(TARGET_DIR)/../$(TARGET_INC_DIR)

create_dir:
	if [ ! -d $(TARGET_DIR)/../$(TARGET_INC_DIR) ]; then \
		mkdir -p $(TARGET_DIR)/../$(TARGET_INC_DIR); \
	fi
	
	if [ ! -d $(TARGET_DIR)/../$(TARGET_LIB_DIR) ]; then \
		mkdir -p $(TARGET_DIR)/../$(TARGET_LIB_DIR); \
	fi