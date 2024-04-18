/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/12 16:41:56 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/04/18 13:58:39 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	i = 0;
	while (paths[i])
	{
		pathpart = ft_strjoin(paths[i], "/");
		path = ft_strjoin(pathpart, cmd);
		free(pathpart);
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

void	ft_execute(char *argv, char **envp)
{
	char	**cmd;
	char	*path;
	int		i;
	pid_t	pid;

	cmd = ft_split(argv, ' ');
	path = ft_findpath(cmd[0], envp);
	i = 0;
	if (path == NULL)
	{
		ft_free(cmd);
		perror("Command not found");
		// exit(127);
	}
	pid = fork();
	if (pid == 0)
	{
		if (execve(path, cmd, envp) == -1)
		{
			ft_free(cmd);
			free(path);
			perror("Execution failed");
			// exit(1);
		}
	}
	else
		waitpid(pid, NULL, 0);
	printf("check\n\n");
	ft_free(cmd);
	free(path);
}

void	ft_childproces(t_command *command, char **envp)
{	
	if (command->infile)
	{
		command->infile_fd = open(command->infile, O_RDONLY, 0777);
		if (command->infile_fd == -1)
		{
			perror("Opening Infile fssssailed");
			// exit(1);
		}
		dup2(command->infile_fd, STDIN_FILENO);
	}
	if (command->pipe)
	{
		dup2(command->fd[1], STDOUT_FILENO); //kan fou gaan
		close(command->fd[0]);
		close(command->fd[1]);
	}
	if (command->outfile && !command->pipe)
	{
		command->outfile_fd = open(command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (command->outfile_fd == -1)
		{
			perror("Opening Outfile failed");
			// exit(1);
		}
		dup2(command->outfile_fd, STDOUT_FILENO);
	}
	ft_execute(command->commands->str, envp);
	if (command->infile)
		close(command->infile_fd);
	if (command->outfile)
		close(command->outfile_fd);
}

void	ft_parentproces(t_command *command, char **envp)
{
	int	std_out;
	int	std_in;

	if (command->outfile)
	{
		command->outfile_fd = open(command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (command->outfile_fd == -1)
		{
			perror("Opening Outfile failed");
			// exit(1);
		}
		std_out = dup(STDOUT_FILENO);
		dup2(command->outfile_fd, STDOUT_FILENO);
	}
	std_in = dup(STDIN_FILENO);
	dup2(command->fd[0], STDIN_FILENO);
	close(command->fd[1]);
	close(command->fd[0]);
	ft_execute(command->commands->next->str, envp);
	if (command->outfile)
	{
		close(command->outfile_fd);
		dup2(std_out, 1);
	}
	dup2(std_in, STDIN_FILENO);
}

// void	ft_handlepipe(t_command *command, char **envp)
// {
// 	pid_t	pid;
// 	pid_t	pid2;

// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		pid2 = fork();
// 		if (pid2 == 0)
// 			ft_childproces(command, envp);
// 		else
// 		{
// 			waitpid(pid2, NULL, 0);
// 			ft_parentproces(command, envp);
// 		}
// 	}
// 	else
// 		waitpid(pid, NULL, 0);
// }

int	pipex(t_command *command, char **envp)
{
	pid_t	pid;

	if (command->pipe)
	{
		if (pipe(command->fd) == -1)
		{
			perror("Pipe Failed");
			// exit(1);
		}
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Fork Failed");
		// exit(1);
	}
	if (pid == 0)
		ft_childproces(command, envp);
	else
	{
		waitpid(pid, NULL, 0);
		if (command->pipe)
			ft_parentproces(command, envp);
	}
	return (1);
}
