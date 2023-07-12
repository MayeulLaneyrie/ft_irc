# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlaneyri <mlaneyri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/15 12:10:07 by mlaneyri          #+#    #+#              #
#    Updated: 2023/07/12 17:18:09 by mlaneyri         ###   ########.fr        #
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
			@ echo "\n\e[3mLinking...\e[0m\n"
			@ $(CC) $(CFLAGS) -o $(NAME) $(SRC)
			@ echo "\e[1mDone!\e[0m"

obj		:
				@ if [ ! -d "./.obj/src" ]; then \
					mkdir -p .obj/src; \
				fi

.obj/%.o	:	%.cpp
			@ echo "\e[3mCompiling $<...\e[0m"
			@ $(CC) -o $@ -c $< $(CFLAGS) $(CDEP)

clean	:
			@ echo "\e[3mCleaning...\e[0m"
			@ $(RM) $(OBJ)
			@ $(RM) .obj

fclean	:	clean
			@ echo "\e[3mRemoving $(NAME)...\e[0m"
			@ $(RM) $(NAME)

re		:	fclean all

.PHONY	:	all obj clean fclean re
