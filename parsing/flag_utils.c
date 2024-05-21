/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   flag_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/22 16:27:04 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/21 16:16:47 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int ft_flags(char **tokens, int j)
// {
// 	if (tokens[j + 1] == NULL)
// 		return (1);
// 	if (tokens[j + 1] == NULL)
// 	{
// 		tokens[j] = ft_strtrim(tokens[j], "\"");
// 		return (1);
// 	}
// 	if (!ft_strncmp(tokens[j + 1], "|", 2))
// 		return (1);
// 	if (!ft_strncmp(tokens[j + 1], ">", 2))
// 		return (1);
// 	return (0);
// }

void	ft_checkflags(t_command *command, int j)
{
	t_list	*lastnode;
	
	if (command->tokens[j][0] == '\0')
		return ;
	if (command->commands == NULL)
	{
		lastnode = malloc(sizeof(t_list));
		if (!lastnode)
			ft_mallocfail(command, "MALLOC FAIL");
		lastnode->str = ft_safe_strdup(command->tokens[j], command);
		lastnode->infileindex = -1;
		lastnode->outfileindex = -1;
		lastnode->infile_fd = -1;
		lastnode->outfile_fd = -1;
		lastnode->next = NULL;
		lastnode->filesset = false;
		lastnode->cmd_delimiter = NULL;
		command->commands = lastnode;
		command->cmd_count ++;
		return ;
	}
	lastnode = command->commands;
	while (lastnode->next != NULL)
			lastnode = lastnode->next;
	lastnode->str = ft_strjoin(lastnode->str, " ");						//protec
	lastnode->str = ft_strjoin(lastnode->str, command->tokens[j]);		//protec
}
