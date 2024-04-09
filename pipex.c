/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/12 16:41:56 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/04/02 14:04:27 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

	cmd = ft_split(argv, ' ');
	path = ft_findpath(cmd[0], envp);
	i = 0;
	if (path == NULL)
	{
		ft_free(cmd);
		perror("Command not found");
		exit(127);
	}
	if (execve(path, cmd, envp) == -1)
	{
		ft_free(cmd);
		free(path);
		perror("Execution failed");
		exit(1);
	}
	ft_free(cmd);
	free(path);
}

void	ft_childproces(int *fd, char **argv, char **envp)
{
	int	infile;

	infile = open(argv[0], O_RDONLY, 0777);
	if (infile == -1)
	{
		perror("Opening Infile failed");
		exit(1);
	}
	dup2(fd[1], STDOUT_FILENO);
	dup2(infile, STDIN_FILENO);
	close(fd[0]);
	ft_execute(argv[1], envp);
	close(infile);
}

void	ft_parentproces(int *fd, char **argv, char **envp)
{
	int	outfile;

	outfile = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
	{
		perror("Opening Outfile failed");
		exit(1);
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(fd[1]);
	ft_execute(argv[2], envp);
	close(outfile);
}

int	pipex(char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid;

	if (argv[1][0] == '\0' || argv[2][0] == '\0')
		return (0);
	if (pipe(fd) == -1)
	{
		perror("Pipe Failed");
		exit(1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Fork Failed");
		exit(1);
	}
	if (pid == 0)
		ft_childproces(fd, argv, envp);
	waitpid(pid, NULL, 0);
	ft_parentproces(fd, argv, envp);
	return (1);
}
