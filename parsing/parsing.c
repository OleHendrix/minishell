/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 11:02:47 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/21 14:14:01 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_commands(t_command *command, char **tokens, int i)
{
	if (!ft_strncmp(tokens[i], "|", 2))
	{
		command->newcom = true;
		ft_files_to_com(command);
		command->pipes++;
		command->infilereset = true;
		command->outfilereset = true;
	}
	else if (!ft_strncmp(tokens[i], "<<", 3))
	{
		command->here_doc = true;
		command->delimiter = ft_safe_strdup(tokens[i + 1], command);
		i++;
	}
	else if (!ft_strncmp(tokens[i], "<<", 2))
	{
		command->here_doc = true;
		command->delimiter = ft_safe_strdup(&tokens[i][2], command);
	}
	else
		addcommand(command, i);
	return (i + 1);
}

int	count_commands(t_list **list)
{
	t_list *current;
	int		i;

	i = 0;
	current = *list;
	while (current)
	{
		current = current->next;
		i ++;
	}
	return (i);
}

void	initpids(t_command *command)
{
	if (!command->cmd_count)
		return;
	command->pids = malloc(command->cmd_count * sizeof(pid_t));
	if (!command->pids)
		ft_mallocfail(command, "MALLOC FAILED");
}

void	fill_struct(char *line, t_command *command, char *mode)
{
	init_struct(command);
	if (built_in_perm(command, line))
		return ;
	ft_supersplit(line, ' ', command);
	if (!command->tokens)
		return (ft_putstr_fd("ERROR IN SPLIT", 2));
	init_files(command, command->tokens);
	// command->cmd_count = count_commands(&command->commands);
	initpids(command);
	if (mode && !ft_strncmp(mode, "test", 5))
		printstruct(command);
	else
		pipex(command);
	ft_free_struct(command);
}
