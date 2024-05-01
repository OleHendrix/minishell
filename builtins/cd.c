/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olehendrix <olehendrix@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 14:15:30 by ohendrix          #+#    #+#             */
/*   Updated: 2024/05/01 20:10:30 by olehendrix       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_cd(t_command *command, char *cmd)
{
	if (!cmd)
		cmd = getenv("HOME");
	if (chdir(cmd) == -1)
		perror("fout");
}