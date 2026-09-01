.PHONY: setup sync-devtools

setup:
	-git config --unset-all core.hooksPath
	git submodule update --init --recursive
	pip install pre-commit
	pre-commit install --hook-type pre-commit --hook-type commit-msg
	$(MAKE) sync-devtools

sync-devtools:
	.devtools/scripts/sync-config .devtools .devtools-sync.sha256 .clang-format .clang-tidy

-include .devtools/common.mk
