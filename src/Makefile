SHELL := /bin/bash

CMAKE     ?= cmake
CONFIG    ?= Debug
BUILD_DIR ?= build
COV_BUILD ?= build-coverage

# CMake targets
APP_TARGET  ?= Main
TEST_TARGET ?= run_test

# Install locations
INSTALL_DIR       ?= ./bin
INSTALL_APP_NAME  ?= maze
INSTALL_TEST_NAME ?= run_test

# dist
DIST_DIR     ?= dist
DIST_NAME    ?= maze-$(shell date +%Y%m%d)
DIST_ARCHIVE ?= $(DIST_DIR)/$(DIST_NAME).tar.gz

# Doxygen
DOXYGEN   ?= doxygen
DOXYFILE  ?= Doxyfile
DOC_OUT   ?= docs/doxygen

# Coverage
LCOV     ?= lcov
GENHTML  ?= genhtml
GCOV_DIR := $(COV_BUILD)/coverage
GCOV_INFO_FILE := $(GCOV_DIR)/coverage.info

.PHONY: all configure build run test install uninstall install-libs clean dvi gcov_report dist help clang-format

install-libs:
	@set -e; \
	if [ "$(UNAME_S)" = "Darwin" ]; then \
	  if ! command -v brew >/dev/null 2>&1; then \
	    echo "install-libs: Homebrew не найден (macOS)."; \
	    exit 1; \
	  fi; \
	  brew install \
	    cmake \
	    qt \
	    doxygen \
	    googletest; \
	elif [ "$(UNAME_S)" = "Linux" ]; then \
	  if ! command -v apt-get >/dev/null 2>&1; then \
	    echo "install-libs: apt-get не найден. Установите зависимости вручную."; \
	    exit 1; \
	  fi; \
	  if ! command -v sudo >/dev/null 2>&1; then \
	    echo "install-libs: sudo не найден. Запустите под root или установите sudo."; \
	    exit 1; \
	  fi; \
	  sudo apt-get update -y >/dev/null; \
	  sudo apt-get install -y \
	    build-essential \
	    cmake \
	    doxygen \
	    qt6-base-dev \
	    qt6-base-dev-tools \
	    libgtest-dev; \
	else \
	  echo "install-libs: неподдерживаемая ОС: $(UNAME_S)"; \
	  exit 1; \
	fi

all: run

configure:
	@mkdir -p "$(BUILD_DIR)"
	@$(CMAKE) -S . -B "$(BUILD_DIR)" -DCMAKE_BUILD_TYPE="$(CONFIG)"

build: configure
	@$(CMAKE) --build "$(BUILD_DIR)" --config "$(CONFIG)"

run: configure
	@$(CMAKE) --build "$(BUILD_DIR)" --target "$(APP_TARGET)" --config "$(CONFIG)"
	@set -e; \
	for p in \
	  "$(BUILD_DIR)/$(APP_TARGET)" \
	  "$(BUILD_DIR)/$(CONFIG)/$(APP_TARGET)" \
	  "$(BUILD_DIR)/bin/$(APP_TARGET)" \
	  "$(BUILD_DIR)/$(CONFIG)/bin/$(APP_TARGET)" \
	  "$(BUILD_DIR)/$(APP_TARGET).app/Contents/MacOS/$(APP_TARGET)" \
	  "$(BUILD_DIR)/$(CONFIG)/$(APP_TARGET).app/Contents/MacOS/$(APP_TARGET)"; do \
	  if [ -x "$$p" ]; then exec "$$p"; fi; \
	done; \
	echo "Executable $(APP_TARGET) not found in $(BUILD_DIR)."; \
	exit 1

test: configure
	@$(CMAKE) --build "$(BUILD_DIR)" --target "$(TEST_TARGET)" --config "$(CONFIG)"
	@set -e; \
	for p in \
	  "$(BUILD_DIR)/$(TEST_TARGET)" \
	  "$(BUILD_DIR)/$(CONFIG)/$(TEST_TARGET)" \
	  "$(BUILD_DIR)/bin/$(TEST_TARGET)" \
	  "$(BUILD_DIR)/$(CONFIG)/bin/$(TEST_TARGET)" \
	  "$(BUILD_DIR)/$(TEST_TARGET).app/Contents/MacOS/$(TEST_TARGET)" \
	  "$(BUILD_DIR)/$(CONFIG)/$(TEST_TARGET).app/Contents/MacOS/$(TEST_TARGET)"; do \
	  if [ -x "$$p" ]; then exec "$$p"; fi; \
	done; \
	echo "Executable $(TEST_TARGET) not found in $(BUILD_DIR)."; \
	exit 1


install: configure
	@$(CMAKE) --build "$(BUILD_DIR)" --target "$(APP_TARGET)" --config "$(CONFIG)"
	@mkdir -p "$(INSTALL_DIR)"
	@set -e; \
	src=""; \
	for p in \
	  "$(BUILD_DIR)/$(APP_TARGET)" \
	  "$(BUILD_DIR)/$(CONFIG)/$(APP_TARGET)" \
	  "$(BUILD_DIR)/bin/$(APP_TARGET)" \
	  "$(BUILD_DIR)/$(CONFIG)/bin/$(APP_TARGET)" \
	  "$(BUILD_DIR)/$(APP_TARGET).app/Contents/MacOS/$(APP_TARGET)" \
	  "$(BUILD_DIR)/$(CONFIG)/$(APP_TARGET).app/Contents/MacOS/$(APP_TARGET)"; do \
	  if [ -x "$$p" ]; then src="$$p"; break; fi; \
	done; \
	if [ -z "$$src" ]; then \
	  echo "Executable $(APP_TARGET) not found in $(BUILD_DIR)."; exit 1; \
	fi; \
	cp -f "$$src" "$(INSTALL_DIR)/$(INSTALL_APP_NAME)"

uninstall:
	@rm -f "$(INSTALL_DIR)/$(INSTALL_APP_NAME)" "$(INSTALL_DIR)/$(INSTALL_TEST_NAME)" 2>/dev/null || true

clean:
	@rm -rf "$(BUILD_DIR)" "$(COV_BUILD)" docs "$(DIST_DIR)" cmake-*

dvi:
	@set -e; \
	if ! command -v "$(DOXYGEN)" >/dev/null 2>&1; then exit 1; fi; \
	if [ ! -f "$(DOXYFILE)" ]; then \
		"$(DOXYGEN)" -g "$(DOXYFILE)" >/dev/null; \
		$(SED_INPLACE) 's|^PROJECT_NAME.*|PROJECT_NAME = "3DViewer_v2.0"|' "$(DOXYFILE)"; \
		$(SED_INPLACE) 's|^OUTPUT_DIRECTORY.*|OUTPUT_DIRECTORY = $(DOC_OUT)|' "$(DOXYFILE)"; \
		$(SED_INPLACE) 's|^INPUT .*|INPUT = core parser render view resources test run_program.cpp run_tests.cpp|' "$(DOXYFILE)"; \
		$(SED_INPLACE) 's|^RECURSIVE.*|RECURSIVE = YES|' "$(DOXYFILE)"; \
		$(SED_INPLACE) 's|^EXTRACT_ALL.*|EXTRACT_ALL = YES|' "$(DOXYFILE)"; \
		$(SED_INPLACE) 's|^GENERATE_HTML.*|GENERATE_HTML = YES|' "$(DOXYFILE)"; \
		$(SED_INPLACE) 's|^GENERATE_LATEX.*|GENERATE_LATEX = NO|' "$(DOXYFILE)"; \
		$(SED_INPLACE) 's|^QUIET.*|QUIET = YES|' "$(DOXYFILE)"; \
	fi; \
	mkdir -p "$(DOC_OUT)"; \
	"$(DOXYGEN)" "$(DOXYFILE)" >/dev/null; \
	echo "$(DOC_OUT)/html/index.html"

gcov_report:
	@mkdir -p "$(COV_BUILD)"
	@$(CMAKE) -S . -B "$(COV_BUILD)" \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_C_FLAGS="--coverage" \
		-DCMAKE_CXX_FLAGS="--coverage" \
		-DCMAKE_EXE_LINKER_FLAGS="--coverage" \
		-DCMAKE_SHARED_LINKER_FLAGS="--coverage"

	@$(CMAKE) --build "$(COV_BUILD)" --target "$(TEST_TARGET)" --config Debug

	@set -e; \
	for p in \
	  "$(COV_BUILD)/$(TEST_TARGET)" \
	  "$(COV_BUILD)/Debug/$(TEST_TARGET)" \
	  "$(COV_BUILD)/bin/$(TEST_TARGET)" \
	  "$(COV_BUILD)/Debug/bin/$(TEST_TARGET)" \
	  "$(COV_BUILD)/$(TEST_TARGET).app/Contents/MacOS/$(TEST_TARGET)" \
	  "$(COV_BUILD)/Debug/$(TEST_TARGET).app/Contents/MacOS/$(TEST_TARGET)"; do \
	  if [ -x "$$p" ]; then "$$p" || true; break; fi; \
	done

	@if ! command -v "$(LCOV)" >/dev/null 2>&1; then exit 1; fi
	@if ! command -v "$(GENHTML)" >/dev/null 2>&1; then exit 1; fi

	@mkdir -p "$(GCOV_DIR)"
	@$(LCOV) --directory "$(COV_BUILD)" --capture \
		--output-file "$(GCOV_INFO_FILE)" \
		--ignore-errors inconsistent,gcov,unsupported,format >/dev/null

	@$(LCOV) --remove "$(GCOV_INFO_FILE)" \
		"/usr/*" \
		"/System/*" \
		"/Library/*" \
		"/opt/homebrew/*" \
		"*/_deps/*" \
		"*/CMakeFiles/*" \
		"*/moc_*" \
		"*/qrc_*" \
		"*/gtest/*" \
		"*/gmock/*" \
		--output-file "$(GCOV_INFO_FILE)" \
		--ignore-errors inconsistent,gcov,unsupported,format,unused >/dev/null

	@$(GENHTML) "$(GCOV_INFO_FILE)" --output-directory "$(GCOV_DIR)" \
		--ignore-errors inconsistent,gcov,unsupported,format,category,unused >/dev/null

	@echo "$(GCOV_DIR)/index.html"

dist:
	@mkdir -p "$(DIST_DIR)"
	@cd .. && tar \
		--exclude=$(notdir $(BUILD_DIR)) \
		--exclude=$(notdir $(COV_BUILD)) \
		--exclude=$(notdir $(DIST_DIR)) \
		--exclude-vcs \
		-czf src/$(DIST_ARCHIVE) src

clang-format:
	 @set -e; \
	 find core parser render view test -type f \( \
	   -name "*.c" -o -name "*.cc" -o -name "*.cpp" -o -name "*.cxx" -o \
	   -name "*.h" -o -name "*.hh" -o -name "*.hpp" -o -name "*.hxx" \
	 \) -print0 | xargs -0 clang-format -i -style=Google

help:
	@echo "Targets: install-libs, all/run, build, test, install, uninstall, dvi, gcov_report, dist, clean"
