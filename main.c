/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/22 14:27:30 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/22 15:31:08 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_input(char **envp, char *mode)
{
	t_command	command;

	command.envp = ft_envdup(envp);
	command.exitstatus = -1;
	while (1)
	{
		command.line = readline("minishell: ");
		if (command.line[0] != '\0')
		{		
			if (!ft_strncmp(command.line, "exit", 5))
			{
				free_ptr_ptr(command.envp);
				exit(EXIT_SUCCESS);
			}
			add_history(command.line);
			command.line = ft_strtrim(command.line, " ");
			// if (!checksyntax(command.line))
			// {
			// 	perror("Wrong syntax");
			// 	free(command.line);
			// }
			// else 
			fill_struct(command.line, &command, mode);
		}
		else
			free(command.line);
	}
	free(command.line);
	ft_free_struct(&command);
}

int main(int argc, char **argv, char **envp)
{
	init_signals();
	if (!envp || argc < 1)
		return (1);
	parse_input(envp, argv[1]);
}