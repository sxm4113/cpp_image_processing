INCLUDE_DIRS = -I/usr/local/include/opencv2 -I./include/
LIBDIR = -L. -lgtest -lgtest_main
CXXFLAGS += -O2 -DLINUX -Wall $(INCLUDE_DIRS) ${LIBDIR} `pkg-config --cflags opencv`
LDFLAGS = `pkg-config --libs opencv`
OBJS=unittest.o
TARGET=unittest

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)  -lgtest -lpthread

clean:
	rm -f *.o
	rm -f $(TARGET)
