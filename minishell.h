#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <limits.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "./Libft/libft.h"
# include "./pipex.h"

typedef struct t_command
{
	char	*infile;
	char	**commands;
	char 	*outfile;
	char	here_doc;
	char	*delimiter;
} t_command;

int	pipex(char **argv, char **envp);

#endif
