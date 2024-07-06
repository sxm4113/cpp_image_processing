INCLUDE_DIRS = -I/usr/local/include/opencv2
CXXFLAGS += -O2 -DLINUX -Wall $(INCLUDE_DIRS) `pkg-config --cflags opencv`
LDFLAGS = `pkg-config --libs opencv` 
OBJS=cpp_image_processing.o Image.o Local_contrast_enhancement.o   
TARGET=cpp_image_processing

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ $(CXXFLAGS) $(OBJS) -o $@ $(LFLAGS) $(LDFLAGS)
 
 
clean:
	rm -f *.o 
	rm -f $(TARGET)
