
## Makefile knowlege
##
##  $< 依赖文件
##  $@ 目标文件
##

CXX = g++
CXXFLAGS = -O0 -Wall -g2 -ggdb -std=c++11 -c -lasan 

### 要链接的库

DIR_OBJS = ./temporary
## 获取 obj 文件
COMMON_SRC = $(wildcard common/*.cpp)
COMMON_OBJ = $(patsubst %.cpp,%.o, $(COMMON_SRC))

TEST_SRC = $(wildcard test/*.cpp)
TEST_OBJ = $(patsubst %.cpp,%.o, $(TEST_SRC))

GATEAPP_SRC = $(wildcard gateapp/*.cpp)
GATEAPP_OBJ = $(patsubst %.cpp,%.o, $(GATEAPP_SRC))

##  增加前缀, 得到 obj文件的存放路径
COMMON_OBJ    	:= $(addprefix $(DIR_OBJS)/, $(COMMON_OBJ))
TEST_OBJ   		:= $(addprefix $(DIR_OBJS)/, $(TEST_OBJ))
GATEAPP_OBJ   	:= $(addprefix $(DIR_OBJS)/, $(GATEAPP_OBJ))

##
ALL_OBJ = $(COMMON_OBJ) $(TEST_OBJ) $(GATEAPP_OBJ)

BIN_HOME = ./bin
TESTAPP = $(BIN_HOME)/testapp
GATEAPP = $(BIN_HOME)/gateapp

ALL_APP = $(BIN_HOME)/testapp $(BIN_HOME)/gateapp

$(DIR_OBJS)/%.o:%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

all: $(ALL_OBJ)
	$(CXX) $(TEST_OBJ) $(COMMON_OBJ) -lasan -lpthread -o $(TESTAPP)
	$(CXX) $(GATEAPP_OBJ) $(COMMON_OBJ) -lasan -lpthread -o $(GATEAPP)

.PHONY: show
show:
	echo "test_src" $(TEST_SRC)
	echo "ALL_OBJ:" $(ALL_OBJ)
	echo "TEST_OBJ:" $(TEST_OBJ)
	echo "COMMON_OBJ:" $(COMMON_OBJ)

.PHONY: clean
clean:
	rm  $(BIN_HOME)/testapp
	rm  $(BIN_HOME)/gateapp
	rm $(ALL_OBJ)
