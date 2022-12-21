# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/09 15:07:52 by lucas-ma          #+#    #+#              #
#    Updated: 2022/12/21 20:26:59 by lucas-ma         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################ PROGRAM ################

NAME	    =    philo

################ TERMINAL ###############

RMV        =        rm -f
MKD        =        mkdir
PRT        =        printf
MKE        =        make
CPY        =        cp

################# COLORS ################

--GRN    =        \033[32m
--RED    =        \033[31m
--WHT    =        \033[39m

################# DIRS ##################

_SRC    =        philo/
_OBJ 	=		 obj/
_LIB    =        libs/
_BIN    =        ./philo/

############### COMPILER ################

CC      =        cc
CFLAGS      =        -Wall -Werror -Wextra -g -fsanitize=address
#valgrind --leak-check=full --show-leak-kinds=all ./minishell
SRCS    =        $(_SRC)main.c $(_SRC)utils.c $(_SRC)time.c $(_SRC)init_params.c

OBJS    =        $(patsubst $(_SRC)%.c,$(_OBJ)%.o,$(SRCS))
################ RULES ##################

all: $(_BIN)$(NAME)

$(_OBJ)%.o: $(_SRC)%.c
	$(MKD) -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(_BIN)$(NAME): $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o $(_BIN)$(NAME)

################ DEPS ###################

############## STRUCTURE ################

clean:
	$(RMV) -r $(_OBJ)

fclean: clean
	$(RMV) -r $(_BIN)$(NAME)

re: fclean all

.PHONY: all clean fclean re