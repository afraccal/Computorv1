NAME	= computorv
CC		= gcc
FLAGS	= -Wall -Wextra -Werror
RM		= rm -f

SRC			= main.c

RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
RESET	= \033[0;0m

OBJ		= $(SRC:.c=.o)

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@printf "$(GREEN)COMPUTORV  CREATED  SUCCESSUFULLY\n$(RESET)"

all: $(NAME)

clean:
	@$(RM) $(OBJ)
	@printf "$(RED)Object files removed\n$(RESET)"
	
fclean: clean
	@$(RM) $(NAME)
	@printf "$(RED)COMPUTORV REMOVED\n$(RESET)"
	
re: fclean all

test: fclean linux clean

leaks: re
	@valgrind --leak-check=full ./$(NAME)

.PHONY: all clean fclean re test