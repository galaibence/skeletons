CXX         := zapcc++
CXXFLAGS    := -O3 -march=core-avx2 -std=c++1z -stdlib=libc++ -Wall -pedantic
LDFLAGS     := -fuse-ld=lld -flto -L/usr/local/lib -lc++ -lc++abi -lGLEW -lglfw -lGL
INCFLAGS    := -I./include

TARGET      := main
MODULES     := . \

SRC_DIR     := $(addprefix ./,$(MODULES))
INC_DIR     := $(addprefix ./,$(MODULES))
BUILD_DIR   := $(addprefix ./,$(MODULES))
RSRC_DIR    := ./resources

SOURCES	  	:= $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cc))
INCLUDES    := $(foreach idir,$(INC_DIR),$(wildcard $(idir)/*.h))
OBJECTS 	  := $(patsubst ./%.cc, ./%.o,$(SOURCES))

first: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $< -o $@

clean:
	rm -f ./$(TARGET)
	find ./ -type f -name '*.o' -delete

format: $(SOURCES) $(INCLUDES)
	clang-format -style=Google -i $(SOURCES)
	clang-format -style=Google -i $(INCLUDES)

run:
	./$(TARGET)
