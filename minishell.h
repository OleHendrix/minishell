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

typedef struct t_list t_list;

typedef struct t_command
{
	char	*infile;
	int		infile_fd;
	char 	*outfile;
	int		outfile_fd;
	struct t_list	*commands;
	bool	here_doc;
	char	*delimiter;
	bool	pipe;
	int		fd[2];
} t_command;

typedef struct t_list
{
	struct t_list	*next;
	char	*str;
}	t_list;

int		pipex(t_command *command, char **envp);
void	printstruct(t_command *command);
void	init_command(t_command *command);
void	printstack(t_list **a);
void	ft_addnode(t_list **list, char *token);
void	combine_node(t_list **list);
int		ft_checkflags(t_list **command, char **tokens, int j);
void	addcommand(t_list **command, char **tokens, int j);
void	init_files(t_command *command, char **tokens);
int 	ft_flags(char **tokens, int j);
void	init_commands(t_command *command, char **tokens);
void	fill_struct2(char *line, char **envp);
void	ft_parsing(char *line, char **envp);
void	ft_childproces(t_command *command, char **envp);
void	ft_parentproces(t_command *command, char **envp);
void	ft_execute(char *argv, char **envp);
char	*ft_findpath(char *cmd, char **envp);
void	ft_free(char **cmd);
void 	fill_struct2(char *line, char **envp);


#endif