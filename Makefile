CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic-errors -I /opt/homebrew/opt/python@3.11/Frameworks/Python.framework/Versions/3.11/include/python3.11
LDFLAGS = -L /opt/homebrew/opt/python@3.11/Frameworks/Python.framework/Versions/3.11/lib -lpython3.11

SRC = symnmf.c  # Add all your source files here
OBJ = $(SRC:.c=.o)
MODULE = symnmf

all: $(MODULE)

$(MODULE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -shared -o $(MODULE).so $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(MODULE).so
