/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/18 16:28:36 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/22 11:51:07 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

pid_t ft_fork(t_command *command)
{
	// pid_t	pid;

	// pid = fork();
	// if (pid == -1)
	// {
	// 	perror("FORK FAILED");
	// }
	// return (pid);

	command->pids[command->cmd_tracker] = fork();
	if (command->pids[command->cmd_tracker] == -1)
		ft_exit(command, "ERROR IN FORK");
	return (command->pids[command->cmd_tracker]);
}

char *getcommand(t_command *command)
{
	int		i;
	t_list	*current;

	i = 0;
	current = command->commands;
	if (!current)
		return (NULL);
	while (i < command->cmd_tracker)
	{
		current = current->next;
		i ++;
	}
	return (current->str);
}

int *create_pipe(t_command *command)
{
	int	*fd;

	fd = malloc(2 * sizeof(int));
	if (!fd)
		ft_mallocfail(command, "ERROR IN PIPE MALLOC");
	if (pipe(fd) == -1)
		ft_exit(command, "ERROR IN MAKING PIPE");
	return (fd);
}

void	ft_free(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

char	*ft_strjoin2(t_command *command, char *s1, char const *s2)
{
	char	*string;
	int		i;
	int		j;

	string = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!string)
		ft_mallocfail(command, "MALLOC FAILED IN STRJOIN2");
	i = 0;
	j = 0;
	if (s1)
	{
		while (s1[i] != '\0')
		{
			string[i] = s1[i];
			i++;
		}
	}
	while (s2[j] != '\0')
	{
		string[i + j] = s2[j];
		j++;
	}
	string[i + j] = '\0';
	return (string);
}

