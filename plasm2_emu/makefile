# https://spin.atomicobject.com/2016/08/26/makefile-c-projects/ :)
TARGET_EXEC := plasm2_emu64
BUILD_DIR := ./../build/emu
SRC_DIRS := ./
SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
UNAME_S := $(shell uname -s)
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP
ifeq ($(UNAME_S),Linux)
        CPPFLAGS += -lSDL2 -I /usr/include/SDL2/ -lm -ldl
		LNKFLAGS := -lSDL2 -lm -ldl
endif
ifeq ($(UNAME_S),Darwin)
        CPPFLAGS += -I"/opt/homebrew/Cellar/sdl2/2.28.5/include/SDL2/"
		LNKFLAGS := -lSDL2 -L"/opt/homebrew/Cellar/sdl2/2.28.5/lib/"
endif
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LNKFLAGS)
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
-include $(DEPS)
