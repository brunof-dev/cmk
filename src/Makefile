SDK_DIR 		:= /opt/intel/openvino_2020.1.023
OPEN_CV_DIR 	:= $(SDK_DIR)/opencv

OPEN_CV_INC_DIR := $(OPEN_CV_DIR)/include
OPEN_CV_LIB_DIR := $(OPEN_CV_DIR)/lib

CC		:= /usr/bin/gcc
CFLAGS 	:= -std=c++11 -I$(OPEN_CV_INC_DIR) -Wall -g
LFLAGS 	:= -lstdc++ -lm -L$(OPEN_CV_LIB_DIR) -lopencv_core -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui
TARGET 	:= main
OBJ 	:= benchmark.o common.o kalman.o matrix.o mean_shift.o main.o

.PHONY 	:= all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)
	chmod 755 $(TARGET)

%.o: %.cpp %.h behav.h
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f *.o $(TARGET)
