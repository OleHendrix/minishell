/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/22 14:27:30 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/04/22 14:58:34 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_input(char **envp)
{
	char *line;

	while (1)
	{
		line = readline("minishell: ");
		if (!ft_strncmp(line, "exit", 5))
			break ;
		fill_struct(line, envp);
	}
}

int main(int argc, char **argv, char **envp)
{
	(void)argv;
	if (!envp || argc < 1)
		return (1);
	parse_input(envp);
}