# ----------------------------------------------------------------------------
# Makefile per l'autocompilazione del progetto filestat, utilty
# di sistema pensata per il monitoraggio automatico di file
# e direcotry, realizzato per il progetto di Sistemi Operativi Laboratorio.
#
# Realizzato da:
# - Fracesco Pio Stelluti 
# - Francesco Coppola
#
# Anno Accademico 2018/2019, Licenza MIT 2019
# -----------------------------------------------------------------------------

# Main target
PROJECT := filestat

# Compilatore
CC := gcc

# Directory
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj

# Funzioni 'wildcard' e 'patsubst' indicate per la generazione 
# dei file .obj nella directory obj/ 
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Clean
RM := rm -rf

$(PROJECT): $(OBJECTS)
	$(CC) $^ -o $@
	@echo "Il programma filestat è stato compilato con successo!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -I$(SRC_DIR) -c $< -o $@

clean: 
	$(RM) $(OBJ_DIR)/*.o
	$(RM) filestat
	@echo "Clean effettuato con successo!"