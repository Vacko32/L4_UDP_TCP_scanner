BUILD_DIR=build
MAKE_FLAGS=-j6
TEST_FLAGS=-j50 --output-on-failure

.PHONY: all test clean setup zip

all:
	@mkdir -p $(BUILD_DIR)
	@$(MAKE) -C $(BUILD_DIR) $(MAKE_FLAGS)
	@mv ./build/src/app ./

clean:
	@rm -f app

test:
	@ctest $(TEST_FLAGS) --test-dir "$(BUILD_DIR)"

setup:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..

zip:
	zip app.zip app
