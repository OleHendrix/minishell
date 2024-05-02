/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/22 14:27:30 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/02 15:44:41 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_input(char **envp, char *mode)
{
	char 		*line;
	t_command	command;

	command.envp = ft_envdup(envp);
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
			fill_struct(line, &command, mode);
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