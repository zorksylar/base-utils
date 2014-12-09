# dirs
SRC_DIR = base
TST_DIR = test
BLD_DIR = build
OBJ_DIR = $(BLD_DIR)/objs

#check os
OS := $(shell uname)
ifeq ($(OS), Darwin)
	CXX?=clang++
else 
	CXX?=g++
endif

#cxx
CXXFLAGS?= -O3 -DNDEBUG -Wall -Wno-unused-function
LDFLAGS?= 
CXX11FLAGS?= -std=c++11 
ifeq ($(OS), Darwin)
	CXX11FLAGS += -stdlib=libc++	
endif

CXXFLAGS+= $(CXX11FLAGS)
#env
EXTRA_INCLUDES=-I.
SYSLIBS=-lpthread
EXTRA_LIBS=-L./$(BLD_DIR)
#SYS_LIBS= 

#targets
CPP_FILES= $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES=$(addprefix $(OBJ_DIR)/, $(notdir $(CPP_FILES:.cpp=.o)))

TST_FILES= $(wildcard $(TST_DIR)/*.cpp)
TST_OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(TST_FILES:.cpp=.o)))

#make
OUT_LIBA=$(BLD_DIR)/libbase.a
OUT_BIN=$(BLD_DIR)/unittest


all : $(OUT_LIBA) $(OUT_BIN)

-include $(OBJ_FILES:.o=.d)

MKDIR_OBJ : 
	mkdir -p $(OBJ_DIR)

$(OBJ_FILES) : $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp | MKDIR_OBJ
	$(CXX) $(CXXFLAGS) $(EXTRA_INCLUDES) -c -o $@ $<
	$(CXX) $(CXX11FLAGS) -MM -MT  '$(OBJ_DIR)/$*.o' $< > $(@:.o=.d)

$(OUT_LIBA) : $(OBJ_FILES) 
	ar rcs $@ $^

$(TST_OBJS) : $(OBJ_DIR)/%.o : $(TST_DIR)/%.cpp | MKDIR_OBJ
	$(CXX) $(CXXFLAGS) $(EXTRA_INCLUDES) -c -o $@ $<
	$(CXX) $(CXX11FLAGS) $(EXTRA_INCLUDES) -MM -MT '$(OBJ_DIR)/$*.o' $< > $(@:.o=.d)

$(OUT_BIN) : $(TST_OBJS) $(OUT_LIBA)
	$(CXX) $(CXXFLAGS) $(EXTRA_INCLUDES) -o $@ $(TST_OBJS) $(OUT_LIBA) $(EXTRA_LIBS) $(SYSLIBS) $(LDFLAGS)


.PHONY:
clean: 
	rm -rf build/

