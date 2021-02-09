NAME = wolf

CC = gcc

CFLAGS = -Wall -Werror -Wextra

SDLF = -I SDL/SDL2.framework/Headers   

INC = hds/wolft3D.h

SRC = src/main.c src/readmap.c src/ft_strdup.c

FRAM =  -framework SDL2 -F ./SDL/ -framework SDL2_mixer -F ./SDL  -framework SDL2_image -F ./SDL -rpath @loader_path/SDL -lz

OBJ = $(SRC:.c=.o) 

F_OBJ = ./OBJ

all: $(NAME) 
$(NAME):$(OBJ)
		@echo "\033[2;36m"
		@$(CC) $(CFLAGS) $(OBJ) $(LIB) $(FRAM) -o $(NAME)
		@echo "wolf3d_3D: executable file is ready"
		@echo "\033[0m"
clean:
		@echo "\033[2;32m"
		@rm -rf $(OBJ)   2> /dev/null || true
		@echo "wolf3d_3D: all resources deleted"
		@echo "\033[0m"
fclean: clean 
		@echo "\033[2;34m"
		@rm -f $(NAME)  2> /dev/null || true
		@echo "wolf3d_3D: executable file deleted"
		@echo "\033[0m"
		
re: fclean all
