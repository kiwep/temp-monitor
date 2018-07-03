BuildFileName := temp-monitor
BuildDir      := .build
ProductTarget := .build/$(BuildFileName)

CFLAGS := -Wall
CFLAGS += -I/usr/local/include
CFLAGS += -I/usr/include
CFLAGS += -I/usr/local/include/evhtp
CFLAGS += -I/usr/include/evhtp
CFLAGS += -I/usr/local/opt/openssl/include

LFLAGS := -L/usr/local/lib
LFLAGS += -L/usr/local/opt/openssl/lib
LFLAGS += -lc -levent -levent_openssl -levhtp -lcrypto -lssl -lpthread -lwiringPi

CXXFLAGS := ""

#
#
#

rfind = $(shell if [ -d '$(1)' ]; then find '$(1)' -name '$(2)'; fi)

Sources := $(call rfind,.,*.c) $(call rfind,.,*.cpp)
ObjFiles := $(addsuffix .o,$(patsubst ./%,$(BuildDir)/%, $(Sources)))
SourceDirList := $(filter-out .,$(shell find . -type d))

CFLAGS += $(patsubst %,-I%,$(SourceDirList))
LFLAGS += -L$(BuildDir)

#
#
#

all: $(ProductTarget)

# all:
# 	@echo $(ObjFiles)

all-clean: clean all

.PHONY: clean
clean:
	@rm -rf .build/*

$(ProductTarget): $(ObjFiles)
	@mkdir -p $(dir $@)
	@echo "[linking  ] $(notdir $(ProductTarget))"
	@$(CC) $(LFLAGS) $(ObjFiles) -o $(ProductTarget)

$(BuildDir)/%.c.o : %.c
	@mkdir -p "$(dir $@)"
	@echo "[compiling] $(notdir $<)"
	@$(CC) $(CFLAGS) -o $@ -c $<

$(BuildDir)/%.cpp.o : %.cpp
	@mkdir -p "$(dir $@)"
	@echo "[compiling] $(notdir $<)"
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -o $@ -c $<
