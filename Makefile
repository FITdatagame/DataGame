COMPILER = $CXX
LINKER = $CXX
MODULES = game engine
SOURCE_DIR = src
TEMP_DIR = tmp
CFLAGS = -std=c++11 -Wall -pedantic -Wno-long-long -O0 -ggdb
LFLAGS = -lSDL2 -lSDL2_gfx -lSDL2_ttf -lSDL2_image
TARGET = datagame

SRC_DIR = $(addprefix $(SOURCE_DIR)/,$(MODULES))
BUILD_DIR = $(addprefix $(TEMP_DIR)/,$(MODULES))

SOURCES = $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp,$(TEMP_DIR)/%.o,$(SOURCES))
DEPS = $(patsubst $(SOURCE_DIR)/%.cpp,$(TEMP_DIR)/%.d,$(SOURCES))
INCLUDES = $(addprefix -I,$(SRC_DIR))

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(COMPILER) -MMD -MP $(CFLAGS) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: run all checkdirs clean

all: checkdirs $(TARGET)

run: all
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(LINKER) $^ -o $(TARGET) $(LFLAGS)

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f ./$(TARGET)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))

-include $(DEPS)
