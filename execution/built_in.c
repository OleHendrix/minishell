/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   built_in.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/30 14:17:36 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/04/30 14:42:20 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	built_in(t_command *command, char *cmd)
{
	if (!ft_strncmp(cmd, "echo -n", 7) && cmd[8] == '\0')
	{
		ft_echo(command, cmd);
		return (1);
	}
	else if ((!ft_strncmp(cmd, "cd", 2) && (cmd[2] == ' ' || cmd[2] == '\0')))
	{
		ft_echo(command, cmd);
		return (1);
	}
	// else if (!ft_strncmp(cmd, "echo -n", 7) && cmd[8] == '\0')
	// {
	// 	ft_echo(command, cmd);
	// 	return (1);
	// }
	return (0);
}


// ◦ echo with option -n
// ◦ cd with only a relative or absolute path
// ◦ pwd with no options
// ◦ export with no options
// ◦ unset with no options
// ◦ env with no options or arguments
// ◦ exit with no options