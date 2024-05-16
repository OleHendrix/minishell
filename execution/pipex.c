/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/12 16:41:56 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/14 16:40:10 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_findpath(t_command *command, char *cmd, char **envp)
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
		ft_mallocfail(command, "MALLOC FAILED IN FT_FINDPATH");
	i = -1;
	while (paths[++i])
	{
		pathpart = ft_strjoin2(command, paths[i], "/");
		path = ft_safe_strjoin(command, pathpart, cmd);
		if (access(path, F_OK) == 0)
			return (ft_free(paths), path);
		free(path);
	}
	ft_free(paths);
	return (NULL);
}

void	ft_execute(t_command *command)
{
	char	**cmd_split;
	char	*path;
	char 	*cmd;

	cmd = getcommand(command);
	cmd = adjustquotes(cmd);
	if (built_in(command, cmd) > 0)
		return ;
	cmd_split = ft_split(cmd, ' ');
	if (cmd_split == NULL)
		ft_mallocfail(command, "MALLOC FAILED IN SUPERSPLIT2");
	path = ft_findpath(command, cmd_split[0], command->envp);
	if (path == NULL)
	{
		ft_free(cmd_split);
		ft_exit(command, "PATH NOT FOUND");
	}
	if (execve(path, cmd_split, command->envp) == -1)
	{
		ft_free(cmd_split);
		free(path);
		ft_exit(command, "ERROR IN EXECVE");
	}
	ft_free(cmd_split);
	free(path);
}

void	ft_childproces(int fd[2], t_command *command)
{
	t_list	*current;
	int		i;

	current = command->commands;
	i = 0;
	while (i < command->cmd_tracker)
	{
		current = current->next;
		i++;
	}
	if (current->outfileindex != -1)
	{
		close(fd[0]);
		dup2(current->outfileindex, STDOUT_FILENO);	
		close(fd[1]);
	}
	else if (command->cmd_tracker + 1 < command->cmd_count)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
	}
	ft_execute(command);
}

void	ft_configinput(int fd[2], t_command *command)
{
	t_list	*current;
	int		i;

	current = command->commands;
	i = 0;
	while (i < command->cmd_tracker + 1)
	{
		current = current->next;
		i++;
	}
	if (current->infileindex != -1)
	{
		close(fd[1]);
		dup2(current->infileindex, STDIN_FILENO);
		close(fd[0]);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
}

void	pipex(t_command *command)
{
	pid_t		pid;
	int			*fd;
	int			i;

	pid = getpid();
	i = 0;
	if (!config_infiles(command))
		return (perror("FILE ERROR(1)"));
	if (!config_outfiles(command))
		return (perror("FILE ERROR(0)"));
	printf("before: cmdtracker: %d cmd_count: %d\n", command->cmd_tracker, command->cmd_count);
	while (command->cmd_tracker < command->cmd_count && pid != 0)
	{
		fd = create_pipe();
		pid = ft_fork(command);
		command->pids[command->cmd_tracker] = pid;
		if (!pid)
			ft_childproces(fd, command);
		else
			ft_configinput(fd, command);
		command->cmd_tracker ++;
		free(fd);
	}
	printf("after: cmdtracker: %d cmd_count: %d\n", command->cmd_tracker, command->cmd_count);
	while (i <= command->cmd_tracker)
	{
		waitpid(command->pids[i], NULL, 0);
		i++;
	}
	exit(command->exitstatus);
}
