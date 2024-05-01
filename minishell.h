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
# include <signal.h>

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
	char	**tokens;
	int		infile_fd;
	char 	*outfile;
	int		outfile_fd;
	struct t_list	*commands;
	struct t_list	*variables;
	char	**envp;
	bool	inquotes;
	int		cmd_count;
	int		cmd_tracker;
	bool	here_doc;
	char	*delimiter;
	int		pipes;
	bool	newcom;
	int		fd[2];
	pid_t	pid;
} t_command;

typedef struct t_split
{
	int i;
	int j;
	int k;
	bool d_inquotes;
	bool s_inquotes;
}	t_split;

//PARSING.
//checksyntax.c
bool	check_chars(char *line, char c);
bool	check_quotes(char *line);
bool	checksyntax(char *line);

//cmd_utils.c
void 	init_struct(t_command *command);
char	*ft_append(char *token, char *value, int i, int j);
char 	*ft_expandvariable(t_command *command, char *token, int i);
void	ft_variable(t_command *command, int j);
void	addcommand(t_command *command, int j);
void	init_commands(t_command *command, char **tokens);

//exit_utils.c
void	ft_mallocfail(t_command *command, char *str);
void 	free_list(t_list *list);
void 	free_ptr_ptr(char **array);
void	ft_free_struct(t_command *command);

//flag_utils.c
// int 	ft_flags(char **tokens, int j);
void	ft_checkflags(t_command *command, int j);

//ft_supersplit.c
int		ft_countwords(char *s, char c);
void	ft_fill(char *s, char **result, int k, int j);
void	ft_suballoc(char *s, char c, char **result, t_split split);
// char	**ft_supersplit(char *s, char c);
void	ft_supersplit(char *s, char c, t_command *command);

//parsing_utils.c
void	printstruct(t_command *command);
void	printstack(t_list **a);
void	trim_quotes(t_list **list);
char	*ft_safe_strdup(char *str, t_command *command);

//parsing.c
void	ft_set_files(t_command *command, char *str, int i);
void	init_files(t_command *command, char **tokens);
void	combine_empty_quote(t_command *command);
int		count_commands(t_list **list);
void	fill_struct(char *line, t_command *command, char *mode);
void 	parse_input(char **envp, char *mode);

//EXECUTION.
//pipe_utils.c
pid_t 	ft_fork(void);
char	*getcommand(t_command *command);
int		*create_pipe(void);
void	ft_free(char **cmd);
char	*ft_strjoin2(char *s1, char const *s2);

//pipex
char	*ft_findpath(char *cmd, char **envp);
void	ft_execute(char **envp, t_command *command);
void	ft_childproces(int fd[2], char **envp, t_command *command);
void	config_files(t_command *command);
void	pipex(char **envp, t_command *command);

//signals.c
void	sig_handler(int sig, siginfo_t *info, void *context);
void	init_signals();

//built_in.c
int		built_in_perm(t_command *command, char *line);
int		built_in(t_command *command, char *cmd);

//BUILTINS
//echo.c
void	ft_echo(t_command *command, char *cmd);

//cd.c
void	ft_cd(t_command *command, char *cmd);

//export.c
char	**ft_envadd(t_command *command, char **envp, char *cmd);
char	**ft_envdup(char **envp);
size_t	ft_getvar(char *cmd);
void	ft_export(t_command *command, char *cmd);

//unset.c
void	ft_unset(t_command *command, char *cmd);



#endif