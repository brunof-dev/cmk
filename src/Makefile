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
CFLAGS 			:= -std=c++11 -I$(IE_INC_DIR) -I$(OPEN_CV_INC_DIR) -Wall -g -MMD
LFLAGS 			:= -lstdc++ -lm -L$(IE_LIB_DIR) -linference_engine -L$(NGRAPH_LIB_DIR) -lngraph -L$(TBB_LIB_DIR) -ltbb\
	       		   -L$(OPEN_CV_LIB_DIR) -lopencv_core -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui

SRC_DIR			:= ./
OBJ_DIR			:= ./obj
DEP_DIR			:= ./obj
BIN_DIR			:= ./bin

SOURCES 		:= $(wildcard $(SRC_DIR)/*.cpp)
OBJS			:= $(subst $(SRC_DIR),$(OBJ_DIR),$(SOURCES:.cpp=.o))
DEPS			:= $(OBJS:.o=.d)

TARGET 			:= $(BIN_DIR)/cmk

.PHONY 			:= all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) -o $@ $^ $(LFLAGS)
	@chmod 755 $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(DEP_DIR)
	rm -rf $(BIN_DIR)

-include $(DEPS)
