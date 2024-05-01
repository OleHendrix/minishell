/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olehendrix <olehendrix@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 14:15:30 by ohendrix          #+#    #+#             */
/*   Updated: 2024/05/01 20:08:30 by olehendrix       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char **ft_unsetenv(t_command *command, char **envp, int j)
{
    char    **envp2;
    int     count;
    int     i;
    int     k;

    count = 0;
    i = 0;
    k = 0;
    while (envp[count])
        count++;
    envp2 = malloc(count * sizeof(char *));
    if (!envp2)
        ft_mallocfail(NULL, "Mallocfail");
    while (i < count - 1)
    {
        if (i == j)
            k++;
        envp2[i] = ft_safe_strdup(envp[i + k], command);
        i++;
    }
    envp2[count - 1] = NULL;
    return (free(envp), envp2);
}

void    ft_unset(t_command *command, char *cmd)
{
    int i;

    i = 0;
    while (command->envp[i])
    {
        if (!ft_strncmp(command->envp[i], cmd, ft_strlen(cmd)) && command->envp[i][ft_strlen(cmd)] == '=')
            command->envp = ft_unsetenv(command, command->envp, i);
        i++;
    }
}
