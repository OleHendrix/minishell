/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/14 15:47:21 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/14 16:05:13 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *adjustquotes2(char *cmd, int delete, char c)
{
	char	*cmd2;
	int		i;
	int		j;

	cmd2 = malloc(sizeof(char) * (ft_strlen(cmd) - delete) + 1);
	i = 0;
	j = 0;
	if (!cmd2)
		return (NULL);
	while (cmd[i + j])
	{
		if (cmd[i + j] != c)
		{
			cmd2[i] = cmd[i + j];
			i++;
		}
		else
			j++;
	}
	cmd2[i] = '\0';
	return (cmd2);
}

char	*adjustquotes(char *cmd)
{
	int		i;
	char	c;
	int		delete;

	i = 0;
	delete = 0;
	while (cmd[i])
	{
		if ((cmd[i] == '\'' || cmd[i] == '\"'))
			c = cmd[i];
		i++;
	}
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == c)
			delete ++;
		i++;
	}
	return (adjustquotes2(cmd, delete, c));
}