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
# include "./Libft/ft_printf/libftprintf.h"

typedef struct t_command
{
	char	*infile;
	int		infile_fd;
	char 	*outfile;
	int		outfile_fd;
	char	**commands;
	bool	here_doc;
	char	*delimiter;
	bool	pipe;
	int		fd[2];
} t_command;

int		pipex(t_command *command, char **envp);
void	printstruct(t_command *command);
void	init_command(t_command *command);
int		ft_checkflags(t_command *command, int i, char **tokens, int j);
void	addcommand(t_command *command, char **tokens, int j);
void	init_files(t_command *command, char **tokens);
void	init_commands(t_command *command, char **tokens);
void	fill_struct(char *line, char **envp);
void	ft_parsing(char *line, char **envp);
void	ft_childproces(t_command *command, char **envp);
void	ft_parentproces(t_command *command, char **envp);
void	ft_execute(char *argv, char **envp);
char	*ft_findpath(char *cmd, char **envp);
void	ft_free(char **cmd);


#endif