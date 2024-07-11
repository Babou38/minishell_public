# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bchapuis <bchapuis@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/30 22:25:01 by bchapuis          #+#    #+#              #
#    Updated: 2024/01/30 22:25:01 by bchapuis         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
FLAGS = -Wall -Wextra -Werror 
INC = -Iincludes/ -I$(LIB_DIR)/includes 
LIB_DIR = libft/
SRC_DIR = source/
OBJ_DIR = objs/
LIB_NAME = libft.a

SRC_NAME =	main.c\
			exit.c\
			cd.c\
			echo.c\
			env.c\
			pwd.c\
			find_exec.c\
			error.c\
			unset.c\
			export.c\
			lexer.c\
			util.c\
			built_ast.c\
			execute_command.c\
			execute_pipe.c\
			execute_redirection.c\
			execute_ast.c\
			heredoc.c\
			build_exec.c\
			build_cmd_utils.c\
			split_args.c\
			execute_cmd_list.c\
			prompt.c


			

OBJ_NAME = $(SRC_NAME:.c=.o)
OBJ = $(addprefix $(OBJ_DIR),$(OBJ_NAME))

$(OBJ_DIR)%.o: $(SRC_DIR)%.c includes/minishell.h Makefile libft/Makefile libft/includes/libft.h libft/srcs/*.c
	@mkdir -p $(OBJ_DIR)
	@echo "##### Creating" [ $@ ] " #####"
	@cc $(FLAGS) -o $@ -c $< $(INC)

$(NAME): $(OBJ) 
	@make -C $(LIB_DIR) --silent
	@cc -o $(NAME) $(OBJ) -L $(LIB_DIR) -lft -lreadline
	@echo "\033[0;32m##### minishell created #####\033[0m"

all: $(NAME)

clean:
	@make -C $(LIB_DIR) clean  --silent
	@rm -f $(CHECK_OBJ) $(OBJ)
	@rm -r $(OBJ_DIR)
	@echo "\033[0;33m##### Removed object files #####\033[0m"

fclean: clean
	@make -C $(LIB_DIR) fclean  --silent
	@rm -f $(NAME)
	@echo "\033[0;33m##### Removed binary files #####\033[0m"

re: fclean all

.PHONY: all clean fclean re
