/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/22 14:27:30 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/04/30 13:47:59 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_input(char **envp, char *mode)
{
	char *line;

	while (1)
	{
		line = readline("minishell: ");
		if (!ft_strncmp(line, "exit", 5))
			break ;
		add_history(line);
		line = ft_strtrim(line, " ");
		if (!checksyntax(line))
		{
			perror("Wrong syntax");
			free(line);
		}
		else
			fill_struct(line, envp, mode);
	}
	free(line);
}

int main(int argc, char **argv, char **envp)
{
	// init_signals();
	if (!envp || argc < 1)
		return (1);
	parse_input(envp, argv[1]);
}