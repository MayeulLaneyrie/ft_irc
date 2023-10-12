# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lnr <lnr@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/15 12:10:07 by mlaneyri          #+#    #+#              #
#    Updated: 2023/10/11 21:27:55 by lnr              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC		=	src/main.cpp \
			src/Serv.cpp \
			src/User.cpp \
			src/Chan.cpp \
			src/Msg.cpp \
			src/utils.cpp \
			src/namecheck.cpp \
				src/cmds/PART.cpp \
				src/cmds/TOPIC.cpp \
				src/cmds/MODE.cpp \
				src/cmds/PASS.cpp \
				src/cmds/NOTICE.cpp \
				src/cmds/VOID.cpp \
				src/cmds/KILL.cpp \
				src/cmds/QUIT.cpp \
				src/cmds/NICK.cpp \
				src/cmds/INVITE.cpp \
				src/cmds/OPER.cpp \
				src/cmds/JOIN.cpp \
				src/cmds/USER.cpp \
				src/cmds/PRIVMSG.cpp \
				src/cmds/PING.cpp \
				src/cmds/WHOIS.cpp \
				src/cmds/KICK.cpp \
				src/cmds/NAMES.cpp

NAME	=	ircserv

CC		=	c++
CFLAGS	=	-Iinclude -std=c++98 -Wall -Wextra -Werror #-g3 -fsanitize=address
CDEP	=	-MMD

RM		=	rm -rf

OBJ		=	$(SRC:%.cpp=.obj/%.o)

DEP		=	$(SRC:%.cpp=.obj/%.d)

all		:	obj $(NAME)

-include	$(DEP)

$(NAME)	:	$(OBJ)
			@ echo "\n\e[3mLinking...\e[0m\n"
			@ $(CC) $(CFLAGS) -o $(NAME) $(SRC)
			@ echo "\e[1mDone!\e[0m"

obj		:
			@ if [ ! -d "./.obj/src" ]; then \
				mkdir -p .obj/src; \
				mkdir -p .obj/src/cmds; \
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
