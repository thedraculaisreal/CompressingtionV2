main: main
	cc src/main.c src/file_reading.c src/token.c src/compression.c -o build/main -lm
