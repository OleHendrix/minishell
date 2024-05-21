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
#include <sys/types.h>
#include <sys/stat.h>


# include "./Libft/libft.h"
# include "./Libft/ft_printf/libftprintf.h"

typedef struct t_list
{
	struct t_list	*next;
	char	*str;
	int		infileindex;
	int		outfileindex;
	int		infile_fd;
	int		outfile_fd;
	bool	filesset;
	char	*cmd_delimiter; //free
}	t_list;

typedef struct t_command
{
	char	*line;
	char	**tokens;
	struct t_list	*commands;
	char 	**outfiles;
	char	**infiles;
	int		infiletracker;
	int		outfiletracker;
	// struct t_list	*variables;
	int		save_std_in;
	int		save_std_out;
	char	*delimiter;
	char	**envp;
	char	*value;
	int		exitstatus;
	bool	inquotes;
	int		cmd_count;
	int		cmd_tracker;
	bool	here_doc;
	int		pipes;
	bool	newcom;
	int		fd[2];
	pid_t	pid;
	pid_t	*pids;
	bool	infilereset;
	bool	outfilereset;
} t_command;

typedef struct t_split
{
	int i;
	int j;
	int k;
	bool d_inquotes;
	bool s_inquotes;
}	t_split;

void 	parse_input(char **envp, char *mode);

//PARSING.
//checksyntax.c
bool	check_chars(char *line, char c);
bool	check_quotes(char *line);
bool	checksyntax(char *line);

//cmd_utils.c
void 	init_struct(t_command *command);
char	*ft_append(t_command *command, int i, int j, int t_idx);
char	*ft_expandvariable(t_command *command, int i, int t_idx);
void	ft_variable(t_command *command, int j);
// char	*ft_strtrim2(char *str, char c, t_command *command);
void	addcommand(t_command *command, int j);

//exit_utils.c
void	ft_mallocfail(t_command *command, char *str);
void	ft_exit(t_command *command, char *str);
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
void	ft_supersplit(char *s, char c, t_command *command);
// char	**ft_supersplit2(char *s, char c);

//parsing_utils.c
void	printarray(char **list);
void	printstruct(t_command *command);
void	printstack(t_list **a);
// void	trim_quotes(t_list **list);
char	*ft_safe_strdup(char *str, t_command *command);
char	*ft_safe_strjoin(t_command *command, char *s1, char const *s2);

//parsing_files.c
void	ft_files_to_com2(t_command *command, t_list *current);
void	ft_files_to_com(t_command *command);
void	addoutfile(t_command *command, char *str);
void	addinfile(t_command *command, char *str);
int		ft_set_files(t_command *command, char *str, int set);
void	init_files(t_command *command, char **tok);

//parsing.c
int		init_commands(t_command *command, char **tokens, int i);
// void	combine_empty_quote(t_command *command);
int		count_commands(t_list **list);
void	initpids(t_command *command);
void	fill_struct(char *line, t_command *command, char *mode);

//EXECUTION.
//pipe_utils.c
pid_t 	ft_fork(t_command *command);
char	*getcommand(t_command *command);
int		*create_pipe(t_command *command);
void	ft_free(char **cmd);
char	*ft_strjoin2(t_command *command, char *s1, char const *s2);

//pipex
char	*ft_findpath(t_command *command, char *cmd, char **envp);
void	ft_execute(t_command *command);
void	ft_childproces(int fd[2], t_command *command);
void	ft_configinput(int fd[2], t_command *command);
void	pipex(t_command *command);

//exec_utils.c
char    *adjuctquotes3(char *cmd, int delete, char *quotes, char *cmd2);
char    *adjustquotes2(char *cmd, int delete, char *quotes);
char    *adjustquotes(char *cmd);
void    ft_waitpids(t_command *command);
void    ft_restore_in_out(t_command *command);
t_list  *getcommand_node(t_command *command);

//file_utils.c
int		ft_open(char **files, int mode);
int		config_infiles(t_command *command, bool next_cmd);
int		config_outfiles(t_command *command);

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

//env.c
char	*get_value(t_command *command, int index);
char	*ft_getenv(t_command *command, char *variable);
void	ft_env(t_command *command);

#endif