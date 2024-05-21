/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jdijkman <jdijkman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/30 10:59:39 by jdijkman      #+#    #+#                 */
/*   Updated: 2024/05/21 13:32:14 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	switch (sig)
	{
	case SIGINT:
		ft_putstr_fd("Received SIGINT ", 1);
		ft_putnbr_fd(getpid(), 1);
		ft_putchar_fd('\n', 1);
		exit(EXIT_SUCCESS);
		break;
	case SIGQUIT:
		ft_putstr_fd("Received SIGQUIT ", 1);
		ft_putnbr_fd(getpid(), 1);
		exit(EXIT_SUCCESS);
		break;
	default:
		ft_putstr_fd("Received unknown signal\n", 1);
		ft_putnbr_fd(getpid(), 1);
		break;
	}
	return ;
}

void	init_signals()
{
	struct sigaction	sa;
	sa.sa_sigaction = sig_handler;
	sa.sa_flags = 0;
	
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		return;
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigterm");
		return;
	}
	return ;
}