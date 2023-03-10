# mrt::collections

export TOPDIR    := $(shell pwd)
export BUILD_DIR := $(TOPDIR)/build
export PREFIX    := $(BUILD_DIR)

export CXX       := g++-10
export CXXFLAGS  := -std=c++2a -I $(BUILD_DIR)/include
#-fconcepts-diagnostics-depth=4

ifeq ($(DEBUG),1)
$(info [!] Debug on)
CXXFLAGS += -g3 -D_DEBUG
endif

.PHONY: build

build: install_headers
	$(info [+] Building)
	for file in $(TOPDIR)/tests/test_*.cc; do \
		$(CXX) $(CXXFLAGS) $$file -o $(BUILD_DIR)/bin/$$(basename $${file%.*}); \
	done

install_headers: prepare
	$(info [+] Installing headers)
	rm -rf $(BUILD_DIR)/include/mrt
	cp -r include $(BUILD_DIR)/include/mrt

prepare:
	$(info [+] Prepare folders)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/bin
	mkdir -p $(BUILD_DIR)/include

install:
	$(info [+] Installing to $(PREFIX))
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/include
	cp -r include $(PREFIX)/include/mrt

test:
	for test in $(BUILD_DIR)/bin/test_*; do \
		if [ -f $$test ]; then \
			$$test; \
		fi; \
	done

$(V).SILENT:
