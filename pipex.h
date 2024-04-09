/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/12 16:42:08 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/04/02 13:14:45 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>

# include "Libft/libft.h"


void	ft_childproces(int *fd, char **argv, char **envp);
void	ft_parentproces(int *fd, char **argv, char **envp);
void	ft_execute(char *argv, char **envp);
char	*ft_findpath(char *cmd, char **envp);
void	ft_free(char **cmd);

#endif