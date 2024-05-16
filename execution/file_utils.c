/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   file_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/14 15:45:44 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/16 15:22:58 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_open(char **files, int mode)
{
	int i;
	int fd;

	i = 0;
	while (files && files[i])
	{
		if (!mode)
		{
			fd = open(files[i], O_RDONLY, 0777);
			if (fd == -1)
				return (1);
			close(fd);
		}
		else
		{
			fd = open(files[i], O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (fd == -1)
				return (1);
			close(fd);
		}
		i++;
	}
	return (0);
}

int config_infiles(t_command *command)
{
	t_list *current;
	int fd;

	current = command->commands;
	if (ft_open(command->infiles, 0))
		return (0);
	command->save_std_in = dup(STDIN_FILENO);
	if (command->save_std_in == -1)
		ft_exit(command, "ERROR IN DUP");
	while (current != NULL)
	{
		if (current->infileindex != -1)
		{
			current->infileindex = open(command->infiles[current->infileindex], O_RDONLY, 0777);
			if (current->infileindex == -1)
				return (0);
			if (current == command->commands)
			{
				if (dup2(current->infileindex, STDIN_FILENO) == -1)
					ft_exit(command, "ERROR IN DUP25");
			}
		}
		current = current->next;
	}
	return (1);
}

int config_outfiles(t_command *command)
{
	t_list *current;
	int fd;

	current = command->commands;
	command->save_std_out = dup(STDOUT_FILENO);
	if (ft_open(command->outfiles, 1))
		return (0);
	while (current != NULL)
	{
		if (current->outfileindex != -1)
		{
			current->outfileindex = open(command->outfiles[current->outfileindex], O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (current->outfileindex == -1)
				return (0);
		}
		current = current->next;
	}
	return (1);
}
