# generates compile_commands.json
# for clangd / ccls in IDE
@phony setup:
	pio run -t compiledb

@phony upload:
	pio run -t upload

@phony monitor:
	pio run -t monitor
