BUILD_DIR=build
MAKE_FLAGS=-j 6
TEST_FLAGS=-j 50 --output-on-failure

.PHONY: all test

all:
	@mkdir -p $(BUILD_DIR)
	@$(MAKE) -C $(BUILD_DIR) $(MAKE_FLAGS)
	@mv ./build/src/app ./

clean:
	@rm app

test:
	@ctest $(TEST_FLAGS) --test-dir "$(BUILD_DIR)"

setup:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..

zip:
	@echo "Idk what to zip"