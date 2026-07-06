SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
INCLUDES =  -w -Wl -Iglad/include/ -ISDL3/3.4.0/include -Iinclude -Icglm/0.9.6/include 
LIBRARIES = -g -Wall -Wextra -Wpedantic -fsanitize=address,undefined -LSDL3/3.4.0/lib -lSDL3 glad/src/glad.o -Lcglm/0.9.6/lib -lcglm extra_utilities/arena.o extra_utilities/perlin.o

all: main

libs:
	gcc -o glad/src/glad.o $(GLAD)

main: $(OBJ)
	gcc -o main $^ $(LIBRARIES) 
	rm -f $(OBJ)


%.o: %.c
	gcc -o $@ -c $< $(INCLUDES)

clean:
	rm -f main 

.PHONY:	all clean