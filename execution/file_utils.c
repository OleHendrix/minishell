/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   file_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/14 15:45:44 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/14 16:05:19 by ohendrix      ########   odam.nl         */
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
	int i;
	int fd;

	current = command->commands;
	i = 0;
	if (ft_open(command->infiles, 0))
		return (0);
	while (current != NULL)
	{
		if (current->infileindex != -1)
		{
			current->infileindex = open(command->infiles[i], O_RDONLY, 0777);
			if (current->infileindex == -1)
				return (0);
			if (current == command->commands)
				dup2(current->infileindex, STDIN_FILENO);
		}
		current = current->next;
	}
	return (1);
}

int config_outfiles(t_command *command)
{
	t_list *current;
	int i;
	int fd;

	current = command->commands;
	i = 0;
	if (ft_open(command->outfiles, 1))
		return (0);
	while (current != NULL)
	{
		if (current->outfileindex != -1)
		{
			current->outfileindex = open(command->outfiles[i], O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (current->outfileindex == -1)
				return (0);
		}
		current = current->next;
	}
	return (1);
}
