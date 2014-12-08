# dirs
SRC_DIR = base
BLD_DIR = build
OBJ_DIR = $(BLD_DIR)/objs

#cxx
CXX?=g++
CXXFLAGS?= -O3 -DNDEBUG -Wall -Wno-unused-function 
CXXFLAGS += -std=c++0x

#env
EXTRA_INCLUDES=-I. -I./base -I..
#SYS_LIBS= 


#targets
CPP_FILES= $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES=$(addprefix $(OBJ_DIR)/, $(notdir $(CPP_FILES:.cpp=.o)))

#make
OUT_LIBA=$(BLD_DIR)/libbase.a


all : $(OUT_LIBA) 

-include $(OBJ_FILES:.o=.d)


$(OBJ_FILES) : $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp | MKDIR_OBJ
	$(CXX) $(CXXFLAGS) $(EXTRA_INCLUDES) -c -o $@ $<
	$(CXX) -MM -MF $(OBJ_DIR)/$*.d $<

$(OUT_LIBA) : $(OBJ_FILES) 
	 ar rcs $@ $^

MKDIR_OBJ : 
	mkdir -p $(OBJ_DIR)


.PHONY:
clean: 
	rm -rf build/

