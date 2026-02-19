SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
INCLUDES =  -w -Wl -Iglad/include/ -ISDL3/3.4.0/include -Iinclude
LIBRARIES = -LSDL3/3.4.0/lib -lSDL3 glad/src/glad.o

all: main

libs:
	gcc -o glad/src/glad.o $(GLAD)

main: $(OBJ)
	gcc -o main $^ $(LIBRARIES) 

%.o: %.c
	gcc -o $@ -c $< $(INCLUDES)

clean:
	rm -f main $(OBJ)