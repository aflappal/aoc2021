# Implicitly CXX := g++
MY_INCLUDES := $(HOME)/koodi/cpp/lib/include
CXXFLAGS := -std=c++17 -pedantic -pedantic-errors -Wextra -Wall -MMD -iquote $(MY_INCLUDES)
BUILD_DIR := ./build
# Add wanted binary names and their deps here (%.o added automatically for every %)
# Example
# MAINS := guess
# guess_deps := feikki.o
# TESTS := test_guess
MAINS := 01a 01b 02
TESTS :=
TARGETS := $(MAINS) $(TESTS)
BUILD_TARGETS := $(TARGETS:%=$(BUILD_DIR)/%)
SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:src/%.cpp=$(BUILD_DIR)/%.o)
DEP := $(OBJ:%.o=%.d)

.PHONY: all

all: $(BUILD_TARGETS)

-include $(DEP)

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) -c $< $(CXXFLAGS) -o $@

define TARGETS_template =
$(1): $$(BUILD_DIR)/$(1)
$$(BUILD_DIR)/$(1): $$(BUILD_DIR)/$(1).o $$(addprefix $$(BUILD_DIR)/,$$($(1)_deps))
endef

$(foreach main,$(TARGETS),$(eval $(call TARGETS_template,$(main))))

$(BUILD_TARGETS):
	$(LINK.cc) -o $@ $^

.PHONY: test
test: $(TESTS)
	$(foreach test,$^,$(BUILD_DIR)/$(test))

tags: $(SRC)
	ctags $(SRC)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
