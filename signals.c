/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jdijkman <jdijkman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/30 10:59:39 by jdijkman      #+#    #+#                 */
/*   Updated: 2024/04/30 11:59:16 by ohendrix      ########   odam.nl         */
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
		break;
	case SIGTERM:
		ft_putstr_fd("Received SIGTERM\n", 1);
		ft_putnbr_fd(getpid(), 1);
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
	if (sigaction(SIGTERM, &sa, NULL) == -1)
	{
		perror("sigterm");
		return;
	}
	return ;
}