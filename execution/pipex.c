/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olehendrix <olehendrix@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 16:41:56 by ohendrix          #+#    #+#             */
/*   Updated: 2024/04/25 16:35:17 by olehendrix       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_findpath(char *cmd, char **envp)
{
	char	**paths;
	char	*pathpart;
	char	*path;
	int		i;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	if (paths == NULL)
	{
		perror("ERROR IN SPLIT");
		exit(1);
	}
	i = 0;
	while (paths[i])
	{
		pathpart = ft_strjoin2(paths[i], "/");
		path = ft_strjoin(pathpart, cmd);
		if (access(path, F_OK) == 0)
			return (ft_free(paths), path);
		free(path);
		i++;
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (NULL);
}

void	ft_execute(char **envp, t_command *command)
{
	char	**cmd_split;
	char	*path;
	char 	*cmd;

	cmd = getcommand(command);
	cmd_split = ft_split(cmd, ' ');
	if (cmd_split == NULL)
	{
		perror("ERROR IN SPLIT");
		exit(1);
	}
	path = ft_findpath(cmd_split[0], envp);
	if (path == NULL)
	{
		ft_free(cmd_split);
		perror("Command not found");
		exit(127);
	}
	if (execve(path, cmd_split, envp) == -1)
	{
		ft_free(cmd_split);
		free(path);
		perror("Execution failed");
		exit(1);
	}
	ft_free(cmd_split);
	free(path);
}

void	ft_childproces(int fd[2], char **envp, t_command *command)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	ft_execute(envp, command);
}

void	config_files(t_command *command)
{
	if (command->infile)
	{
		command->infile_fd = open(command->infile, O_RDONLY, 0777);
		dup2(command->infile_fd, STDIN_FILENO);
	}
	if (command->outfile)
	{
		command->outfile_fd = open(command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		dup2(command->outfile_fd, STDOUT_FILENO);
	}
}

void	pipex(char **envp, t_command *command)
{
	pid_t		pid;
	int			*fd;

	pid = getpid();
	config_files(command);
	while (command->cmd_tracker < command->cmd_count - 1 && pid != 0)
	{
		fd = create_pipe();
		pid = ft_fork();
		if (!pid)
			ft_childproces(fd, envp, command);
		else
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			waitpid(pid, NULL, 0);
		}
		command->cmd_tracker++;
		free(fd);
	}
	ft_execute(envp, command);
	// free_list(command->commands);
}
