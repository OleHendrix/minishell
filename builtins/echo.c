/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/30 14:15:30 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/04/30 17:00:36 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_echo(t_command *command, char *cmd)
{
	ft_putstr_fd(cmd, STDOUT_FILENO);
}

void ft_cd(t_command *command, char **envp, char *cmd)
{
	if (!cmd)
		cmd = getenv("HOME");
	if (chdir(cmd) == -1)
		perror("fout");
}