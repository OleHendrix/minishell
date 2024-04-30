/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   built_in.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/30 14:17:36 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/04/30 17:02:10 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int built_in_perm(char **envp, char *line)
{
	int	i;
	bool quotes;

	i = 0;
	quotes = false;
	while (line[i])
	{
		if (line[i] == '\"' || line[i] == '\'')
			quotes = !quotes;
		if (line[i] == '|' && !quotes)
			return (0);
		i ++;
	}
	if (!ft_strncmp(line, "cd", 2) && (line[2] == ' ' || line[2] == '\0'))
	{
		if (line[2] == '\0')
			return (ft_cd(NULL, envp, NULL), 1);
		else
			return (ft_cd(NULL, envp, line + 3), 1);
	}
	return (0);
}

int	built_in(t_command *command, char **envp, char *cmd)
{
	if (!ft_strncmp(cmd, "echo -n", 7) && cmd[8] == '\0')
	{
		ft_echo(command, cmd + 7);
		return (1);
	}
	else if ((!ft_strncmp(cmd, "cd", 2) && (cmd[2] == ' ' || cmd[2] == '\0')))
	{
		if (cmd[2] == '\0')
			return (ft_cd(NULL, envp, NULL), 1);
		else
			return (ft_cd(NULL, envp, cmd + 3), 1);
	}
	return (0);
}


// ◦ echo with option -n
// ◦ cd with only a relative or absolute path
// ◦ pwd with no options
// ◦ export with no options
// ◦ unset with no options
// ◦ env with no options or arguments
// ◦ exit with no options