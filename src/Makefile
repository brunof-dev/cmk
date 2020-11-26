SDK_DIR 		:= /opt/intel/openvino_2020.1.023
IE_DIR 			:= $(SDK_DIR)/deployment_tools/inference_engine
OPEN_CV_DIR 	:= $(SDK_DIR)/opencv

IE_INC_DIR 		:= $(IE_DIR)/include
OPEN_CV_INC_DIR := $(OPEN_CV_DIR)/include

IE_LIB_DIR 		:= $(IE_DIR)/lib/intel64
NGRAPH_LIB_DIR 	:= $(SDK_DIR)/deployment_tools/ngraph/lib
TBB_LIB_DIR 	:= $(IE_DIR)/external/tbb/lib
OPEN_CV_LIB_DIR := $(OPEN_CV_DIR)/lib

CC				:= /usr/bin/gcc
CFLAGS 			:= -std=c++11 -I$(IE_INC_DIR) -I$(OPEN_CV_INC_DIR) -Wall -g
LFLAGS 			:= -lstdc++ -lm -L$(IE_LIB_DIR) -linference_engine -L$(NGRAPH_LIB_DIR) -lngraph -L$(TBB_LIB_DIR) -ltbb\
	       		   -L$(OPEN_CV_LIB_DIR) -lopencv_core -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui
TARGET 			:= cmk
OBJ 			:= benchmark.o blob.o blob_data.o cmk.o common.o data.o kalman.o matrix.o mean_shift.o neural.o person.o main.o

.PHONY 			:= all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)
	chmod 755 $(TARGET)

%.o: %.cpp %.h behav.h
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f *.o $(TARGET)
