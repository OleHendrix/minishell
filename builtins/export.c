/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olehendrix <olehendrix@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 14:15:30 by ohendrix          #+#    #+#             */
/*   Updated: 2024/05/01 20:07:36 by olehendrix       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char **ft_envadd(t_command *command, char **envp, char *cmd)
{
	char	**envp2;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (envp[count])
		count++;
	envp2 = malloc((count + 2) * sizeof(char *));
	if (!envp2)
		ft_mallocfail(NULL, "Mallocfail");
	while (i < count)
	{
		envp2[i] = ft_safe_strdup(envp[i], command);
		i++;
	}
	envp2[i] = ft_safe_strdup(cmd, command);
	envp2[count + 1] = NULL;
	return (free(envp), envp2);	
}

char **ft_envdup(char **envp)
{
	char	**envp2;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (envp[count])
		count++;
	envp2 = malloc((count + 1) * sizeof(char *));
	if (!envp2)
		ft_mallocfail(NULL, "Mallocfail");
	while (i < count)
	{
		envp2[i] = ft_safe_strdup(envp[i], NULL); 
		i++;
	}
	envp2[count] = NULL;
	return (envp2);	
}

size_t	ft_getvar(char *cmd)
{
	size_t	i;

	i = 0;
	while (cmd[i] != '\0')
	{
		if (cmd[i] == '=')
			return (i + 1);
		i++;
	}
	return (i);
}

void	ft_export(t_command *command, char *cmd)
{
	size_t	variable;
	int		i;

	variable = ft_getvar(cmd);
	i = 0;
	while (command->envp[i])
	{
		if (!ft_strncmp(command->envp[i], cmd, variable))
		{
			free(command->envp[i]);
			command->envp[i] = ft_safe_strdup(cmd, command);
			return ;
		}
		i++;
	}
	command->envp = ft_envadd(command, command->envp, cmd);
}
