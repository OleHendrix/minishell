/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/22 16:37:48 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/04/30 13:39:40 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_mallocfail(t_command *command, char *str)
{
	perror(str);
	ft_free_struct(command);
	exit(1);
}

void free_list(t_list *list)
{
	t_list *current;
	t_list *next;

	current = list;
	while (current != NULL)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
}

void free_ptr_ptr(char **array)
{
	int	i;

	i = 0;
	while(array[i])
	{
		free(array[i]);
		i ++;
	}
	free(array);
}

void	ft_free_struct(t_command *command)
{
	if (command->infile)
		free(command->infile);
	if (command->outfile)
		free(command->outfile);
	if (command->commands)
		free_list(command->commands);
	if (command->tokens)
		free_ptr_ptr(command->tokens);
	if (command->delimiter)
		free(command->delimiter);
}
