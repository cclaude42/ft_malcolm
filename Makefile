###################################
# ft_malcolm
###################################

NAME = ft_malcolm

CC = clang

CFLAGS = -Wall -Wextra -Werror

INCLUDES = -I includes

FILES = ft_malcolm error tools print

SRC = $(addsuffix .c, $(addprefix srcs/, $(FILES))) \

OBJ = $(SRC:c=o)

###################################
# Methods
###################################

all: $(NAME)

$(NAME): $(OBJ)
	@echo "\033[0;32m\n\nCompiling ft_malcolm..."
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJ)
	@echo "\n\033[0mDone !"

%.o: %.c
	@printf "\033[0;33mGenerating ft_malcolm objects... %-20.20s\r" $@
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "\033[0;31m\nDeleting ft_malcolm objects..."
	@rm -f $(OBJ)
	@echo "\033[0m"

fclean:
	@echo "\033[0;31m\nDeleting ft_malcolm objects..."
	@rm -f $(OBJ)
	@echo "\nDeleting ft_malcolm executable..."
	@rm -f $(NAME)
	@echo "\033[0m"

re: fclean all

.PHONY: clean fclean re
