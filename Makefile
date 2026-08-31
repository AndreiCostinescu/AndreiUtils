.PHONY: setup format lint

BUILD_DIR ?= build

setup:
	-git config --unset-all core.hooksPath
	pip install pre-commit
	pre-commit install --hook-type pre-commit --hook-type commit-msg

format:
	clang-format -i $$(git ls-files '*.h' '*.hpp' '*.hh' '*.hxx' '*.c' '*.cc' '*.cpp' '*.cxx')

lint:
	clang-tidy -p $(BUILD_DIR) $$(git ls-files '*.c' '*.cc' '*.cpp' '*.cxx')