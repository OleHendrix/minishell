NAME = minishell

SOURCES = \
main.c \
pipex.c \
parsing.c \

OBJS = $(SOURCES:.c=.o)

# CFLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJS)
	make -C Libft/ all
	cc $(CFLAGS) $(SOURCES) Libft/libft.a -lreadline -o $(NAME) 

clean:
		rm -f $(OBJS)
		make -C Libft/ clean

fclean: clean
		rm -f $(NAME)
		make -C Libft/ fclean

re: fclean all

.PHONY: all, clean, fclean, re,