SRC_DIR			:= ./
INC_DIR			:= ../include
OBJ_DIR			:= ./obj
DEP_DIR			:= ./obj
LIB_DIR			:= ../lib
BIN_DIR			:= ../bin

IE_INC_DIR 		:= $(INC_DIR)/ie
OPEN_CV_INC_DIR := $(INC_DIR)/opencv

CC				:= /usr/bin/gcc
CFLAGS 			:= -std=c++11 -I$(IE_INC_DIR) -I$(OPEN_CV_INC_DIR) -Wall -g -MMD
LFLAGS 			:= -lstdc++ -lm -L$(LIB_DIR) -linference_engine -lngraph -ltbb -lopencv_core -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui

SOURCES 		:= $(wildcard $(SRC_DIR)/*.cpp)
OBJS			:= $(subst $(SRC_DIR),$(OBJ_DIR),$(SOURCES:.cpp=.o))
DEPS			:= $(OBJS:.o=.d)

TARGET 			:= $(BIN_DIR)/tracker

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
