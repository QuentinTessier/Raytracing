##
## EPITECH PROJECT, 2018
## Engine
## File description:
## Makefile
##

NAME		= 	Raytracer

++			=	g++

INCLUDE		=	-Iinclude -Iengine/include

SFML		=	-lsfml-graphics -lsfml-window -lsfml-system

MATH		=	-lm

CPPFLAGS  	=	-Wall -Wextra $(INCLUDE) $(SFML) $(MATH)

SRC			=	./src/main.cpp

OBJ			=	$(SRC:.cpp=.o)

all:			$(NAME)

$(NAME):		$(OBJ)
				$(++) -o $(NAME) $(OBJ) $(CPPFLAGS)
				rm -f $(OBJ)

clean:
				rm -f $(OBJ)

fclean:			clean
				rm -f $(NAME)

re:				fclean all

debug:			CFLAGS += -g
debug:			re

.PHONY:			all clean fclean re debug