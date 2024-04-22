/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/18 16:28:36 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/04/22 14:33:45 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_list(t_command *command)
{
	t_list *current;
	t_list *next;

	current = command->commands;
	while (current != NULL)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
}

pid_t ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("FORK FAILED");
	}
	// printf("in fork pid: %d\n", pid);
	return (pid);
}

char *getcommand(t_command *command)
{
	int		i;
	t_list	*current;

	i = 0;
	current = command->commands;
	while (i < command->cmd_tracker)
	{
		current = current->next;
		i ++;
	}
	return (current->str);
}

int *create_pipe(void)
{
	int	*fd;

	fd = malloc (2 * sizeof(int));
	if (fd == NULL)
	{
		perror("MALLOC FAILED");
		exit(1);
	}
	if (pipe(fd) == -1)
	{
		perror("PIPE FAILED");
		free(fd);
	}
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
