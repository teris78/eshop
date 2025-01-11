# Ορισμος του compiler
CC = gcc
#flags μεταγλωττισης
CFLAGS =  -g
#αρχεια κωδικα και εκτελεσιμο
SRC = main.c eshop.c
TARGET = eshop
#Δημιουργια εκτελεσιμου
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
#καθαρισμος αρχειων
clean:
	rm -f $(TARGET)

