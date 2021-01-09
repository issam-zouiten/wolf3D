SRC		=	./src/main.c	\
			./src/color.c	\
			./src/readmap.c

NAME	=	wolf3D

CC		=	@gcc 

RM		=	@rm -f

OBJ		=	$(SRC:.c=.o)

CFLAGS	=	-Wall -Wextra -Werror
CFLAGS	+=	-I ./hds/

LDLIBS	=	`sdl2-config --cflags --libs`

all		:	$(NAME)

$(NAME)	:	$(OBJ)
			$(CC) -o $(NAME) $(OBJ) $(LDLIBS)

clean	:
			$(RM) $(OBJ)

fclean	:	clean
			$(RM) $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re
