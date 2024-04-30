/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/22 14:27:30 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/04/29 15:27:23 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	sig_handler(int sig, siginfo_t *info, void *context)
// {
// 	(void)sig;
// 	(void)context;
// 	kill(info->si_pid, SIGUSR1);
// 	ft_putstr_fd("sig rec", 1);
// 	ft_putchar_fd('\n', 1);
	
// }

void parse_input(char **envp, char *mode)
{
	char *line;
	// struct sigaction	sa;
	// sa.sa_sigaction = sig_handler;

	while (1)
	{
		// sigaction(SIGUSR1, &sa, 0);
		line = readline("minishell: ");
		if (!ft_strncmp(line, "exit", 5))
			break ;
		line = ft_strtrim(line, " ");
		if (!checksyntax(line))
			perror("Wrong syntax");
		else
			fill_struct(line, envp, mode);
	}
}

int main(int argc, char **argv, char **envp)
{
	// ft_putnbr_fd(getpid(), 1);
	
	if (!envp || argc < 1)
		return (1);
	parse_input(envp, argv[1]);
}