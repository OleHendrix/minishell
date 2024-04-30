/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   flag_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/22 16:27:04 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/04/29 17:09:35 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_flags(char **tokens, int j)
{
	if (tokens[j + 1] == NULL)
		return (1);
	if (tokens[j + 1] == NULL)
	{
		tokens[j] = ft_strtrim(tokens[j], "\"");
		return (1);
	}
	if (!ft_strncmp(tokens[j + 1], "|", 2))
		return (1);
	if (!ft_strncmp(tokens[j + 1], ">", 2))
		return (1);
	return (0);
}

int	ft_checkflags(t_command *command, int j)
{
	t_list	*lastnode;

	if (command->commands == NULL)
		return (0);
	lastnode = command->commands;
	while (lastnode->next != NULL)
			lastnode = lastnode->next;
	if (ft_flags(command->tokens, j))
	{
		if (j > 0 && ft_strncmp(command->tokens[j - 1], "|", 2)
			&& ft_strncmp(command->tokens[j - 1], "<", 2) &&
			ft_strncmp(command->tokens[j - 1], ">", 2))
		{
			if (command->tokens[j][0] == '\"')
				command->tokens[j] = ft_strtrim(command->tokens[j], "\"");
			lastnode->str = ft_strjoin(lastnode->str, " ");						//protec
			lastnode->str = ft_strjoin(lastnode->str, command->tokens[j]);		//protec
			return (1);
		}
	}
	return (0);
}
