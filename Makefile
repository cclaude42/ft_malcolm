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
	@printf "\033[0;32m\n\nCompiling %s...\n" $(NAME)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJ)
	@printf "\n\033[0mDone !\n"

%.o: %.c
	@printf "\033[0;33mGenerating %s objects... %-20.20s\r" $(NAME) $@
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@printf "\033[0;31m\nDeleting %s objects...\n" $(NAME)
	@rm -f $(OBJ)
	@printf "\033[0m\n"

fclean:
	@printf "\033[0;31m\nDeleting %s objects...\n" $(NAME)
	@rm -f $(OBJ)
	@printf "\nDeleting %s executable...\n" $(NAME)
	@rm -f $(NAME)
	@printf "\033[0m\n"

re: fclean all

setup: re
	rm -rf .vagrant
	vagrant up

.PHONY: clean fclean re setup
