SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
INCLUDES =  -w -Wl -Iglad/include/ -ISDL3/3.4.0/include -Iinclude -Icglm/0.9.6/include
LIBRARIES = -LSDL3/3.4.0/lib -lSDL3 glad/src/glad.o -Lcglm/0.9.6/lib -lcglm

all: main

libs:
	gcc -o glad/src/glad.o $(GLAD)

main: $(OBJ)
	gcc -o main $^ $(LIBRARIES) 

%.o: %.c
	gcc -o $@ -c $< $(INCLUDES)

clean:
	rm -f main $(OBJ)