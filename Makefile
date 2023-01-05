# mrt::collections

export TOPDIR    := $(shell pwd)
export BUILD_DIR := $(TOPDIR)/build

export CXX       := g++-10
export CXXFLAGS  := -std=c++2a -I $(BUILD_DIR)/include
#-fconcepts-diagnostics-depth=4

ifeq ($(DEBUG),1)
CXXFLAGS += -g3 -D_DEBUG
endif

.PHONY: build

build: install_headers
	$(info [+] Building)
	$(CXX) $(CXXFLAGS) test.cc -o $(BUILD_DIR)/bin/test

install_headers: prepare
	$(info [+] Installing headers)
	rm -rf $(BUILD_DIR)/include/mrt
	cp -r include $(BUILD_DIR)/include/mrt

prepare:
	$(info [+] Prepare folders)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/bin
	mkdir -p $(BUILD_DIR)/include

$(V).SILENT:
