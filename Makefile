CXX = g++
CXXFLAGS = -std=c++17 -O2
# -Iimgui -Iimgui/backend
LIBS = -lglfw -lGL -ldl -lm -pthread

SRCS = src/main.cpp src/Application.cpp src/opengl.cpp src/mymath.cpp

OBJS = $(SRCS:%.cpp=bin/%.o)

OUT = human_gl

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	CXXFLAGS += -I/usr/include
	LIBS = -lglfw -lGL -ldl -lm -pthread
else ifeq ($(UNAME_S),Darwin)
	CXXFLAGS += -I/usr/include
	LIBS = -lglfw -framework OpenGL -ldl -lm -pthread
endif

all: bin/$(OUT)

bin/$(OUT): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

bin/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJS) bin/$(OUT)

re: clean all
