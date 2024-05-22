/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/14 15:47:21 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/22 15:35:09 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*adjuctquotes3(char *cmd, int delete, char *quotes, char *cmd2)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	(void)delete;
	while (cmd[i + j])
		{
			if (cmd[i + j] != quotes[0] && cmd[i + j] != quotes[1])
			{
				cmd2[i] = cmd[i + j];
				i++;
			}
			else
				j++;
		}
	cmd2[i] = '\0';
	return (free(quotes), cmd2);
}

char	*adjustquotes2(char *cmd, int delete, char *quotes)
{
	char	*cmd2;
	int		i;
	int		j;

	cmd2 = malloc(sizeof(char) * (ft_strlen(cmd) - delete) + 1);
	i = 0;
	j = 0;
	if (!cmd2)
		return (printf("MALLOC FAILED"), NULL);
	if (quotes[0] == quotes[1])
	{
		while (cmd[i + j])
		{
			if (cmd[i + j] != quotes[0])
			{
				cmd2[i] = cmd[i + j];
				i++;
			}
			else
				j++;
		}
	}
	else 
		return (free(quotes), adjuctquotes3(cmd, delete, quotes, cmd2));
	return (cmd2);
}

char	*get_first_last_quote(char *cmd)
{
	char	*quotes;
	int		i;
	int		end;

	i = 0;
	end = ft_strlen(cmd);
	quotes = ft_calloc (sizeof(char) , 2);
	while (cmd[i ++])
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
		{
			quotes[0] = cmd[i];
			break ;
		}
		if (i == end)
			return (free(quotes), NULL);
	}
	while (end > 0)
	{
		if (cmd[end] == '\''  || cmd[end] == '\"')
		{
			quotes[1] = cmd[end];
			break ;
		}
		end --;
	}
	return (quotes);
}

char	*adjustquotes(char *cmd)
{
	char *quotes;
	int		i;
	int		delete;

	i = 0;
	delete = 0;
	quotes = get_first_last_quote(cmd);
	if (!quotes)
		return (cmd);
	if (quotes[0] == quotes[1])
	{
		while (cmd[i])
		{
			if (cmd[i] == quotes[0])
				delete ++;
			i ++;
		}
	}
	else 
	{
		while (cmd[i])
		{
			if (cmd[i] == quotes[0] || cmd[i] == quotes[1])
				delete ++;
			i++;
		}
	}
	return (adjustquotes2(cmd, delete, quotes));
}

void	ft_waitpids(t_command *command)
{
	int	i;

	i = 0;
	
	while (i < command->cmd_tracker)
	{
		if(command->pids[i])
			waitpid(command->pids[i], &command->exitstatus, 0);
		// printf("%d, %d\n", i, command->cmd_tracker);
		i++;
	}
	// printf(":)\n");
	// waitpid(command->pids[0], NULL, 0);
	// perror("pfnif\n");
}

void	ft_restore_in_out(t_command *command)
{
	if (command->save_std_in > -1)
	{
		if (dup2(command->save_std_in, STDIN_FILENO) == -1)
			ft_exit(command, "ERROR IN DUP26");
	}
	if (command->save_std_out > -1)
	{
		if (dup2(command->save_std_out, STDOUT_FILENO) == -1)
			ft_exit(command, "ERROR IN DUP27");
	}
	close(command->save_std_in);
	close(command->save_std_out);
}

t_list *getcommand_node(t_command *command)
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
	return (current);
}

