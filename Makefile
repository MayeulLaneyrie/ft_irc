# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/15 12:10:07 by mlaneyri          #+#    #+#              #
#    Updated: 2023/07/12 15:57:48 by mlaneyri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC			=	src/main.cpp \
				src/die.cpp \
				src/Server.cpp \
				src/Client.cpp \
				src/Chan.cpp \
				src/Msg.cpp

NAME		=	ircserv

CC			=	c++
CFLAGS		=	-Wall -Wextra -Werror -Iinclude -std=c++98#-g3 -fsanitize=address
CDEP		=	-MMD

RM			=    rm -rf

OBJ		=    $(SRC:%.cpp=.obj/%.o)

DEP		=    $(SRC:%.cpp=.obj/%.d)

all		:	obj $(NAME)

-include $(DEP)

$(NAME)	:	$(OBJ)
			@ echo "\n\e[1mLinking...\e[0m\n"
			$(CC) $(CFLAGS) -o $(NAME) $(SRC)

obj		:
				@ if [ ! -d "./.obj/src" ]; then \
					mkdir -p .obj/src; \
				fi

.obj/%.o	:	%.cpp
			@ echo "\n\e[1mCompiling ft_irc...\e[0m\n"
			$(CC) -o $@ -c $< $(CFLAGS) $(CDEP)

clean	:
			$(RM) $(OBJ)
			$(RM) .obj

fclean	:	clean
	$(RM) $(NAME)

re		:	fclean all

.PHONY	:	all obj clean fclean re
