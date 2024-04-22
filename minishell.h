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

typedef struct t_list
{
	struct t_list	*next;
	char	*str;
}	t_list;

typedef struct t_command
{
	char	*infile;
	int		infile_fd;
	char 	*outfile;
	int		outfile_fd;
	struct t_list	*commands;
	int		cmd_count;
	int		cmd_tracker;
	bool	here_doc;
	char	*delimiter;
	int		pipes;
	int		fd[2];
	pid_t	pid;
} t_command;


void	fill_struct(char *line, char **envp);
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
char	*ft_findpath(char *cmd, char **envp);
void	ft_free(char **cmd);
void	addcommand(t_list **commands, char **tokens, int j);
void	pipex(char **envp, t_command *command);
void	ft_free(char **cmd);
int		*create_pipe(void);
pid_t	ft_fork(void);
char	*getcommand(t_command *command);
void	free_list(t_command *command);



#endif