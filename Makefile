# Makefile pro kompilaci a spuštění WinAPI příkladu

# Jméno výstupního souboru
OUTPUT = winapi_example.exe

# Zdrojový soubor
SOURCE = itu-template.c

# Knihovny
LIBS = -lgdi32 -lopengl32

# Kompilační příkaz
CC = gcc

# Kompilace
$(OUTPUT): $(SOURCE)
	$(CC) -o $(OUTPUT) $(SOURCE) $(LIBS)

# Spuštění
run: $(OUTPUT)
	./$(OUTPUT)

# Čistící příkaz
clean:
	del $(OUTPUT)

.PHONY: clean run
