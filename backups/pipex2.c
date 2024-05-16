/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/12 16:41:56 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/09 15:41:32 by ohendrix      ########   odam.nl         */
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

char **trimcmd(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i][0] == '\"' && cmd[i][ft_strlen(cmd[i]) - 1] == '\"')
			cmd[i] = ft_strtrim(cmd[i], "\""); //protec + trim maar 1 quote 
		else if (cmd[i][0] == '\'' && cmd[i][ft_strlen(cmd[i]) - 1] == '\'')
			cmd[i] = ft_strtrim(cmd[i], "\'");
		i++;
	}
	return (cmd);
}

void	ft_execute(t_command *command)
{
	char	**cmd_split;
	char	*path;
	char 	*cmd;

	cmd = getcommand(command);
	if (built_in(command, cmd) > 0)
		return ;
	cmd_split = ft_supersplit2(cmd, ' ');
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
	int 	i;

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
	else
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	ft_execute(command);
}

int	config_infiles(t_command *command)
{
	t_list	*current;
	int		i;
	int		fd;

	current = command->commands;
	i = 0;
	if (!current)
		return (1);
	while (current != NULL && command->infiles[i])
	{
		while (i < current->infileindex)
		{
			fd = open(command->infiles[i], O_RDONLY, 0777);
			if (fd == -1)
				return (0);
			close(fd);
			i++;
		}
		if (current->infileindex != -1)
		{
			current->infileindex = open(command->infiles[i], O_RDONLY, 0777);
			if (current->infileindex == -1)
				return (0);
		}
		i++;
		current = current->next;
	}
	return (1);
}

int	config_outfiles(t_command *command)
{
	t_list	*current;
	int		i;
	int		fd;

	current = command->commands;
	i = 0;
	if (!current)
		return (1);
	while (current != NULL && command->outfiles[i])
	{
		while (i < current->outfileindex)
		{
			fd = open(command->outfiles[i], O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (fd == -1)
				return (0);
			close(fd);
			i++;
		}
		if (current->outfileindex != -1)
		{
			current->outfileindex = open(command->outfiles[i], O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (current->outfileindex == -1)
				return (0);
		}
		i++;
		current = current->next;
	}
	return (1);
}

int	getinf(t_command *command)
{
	t_list	*current;
	int		i;

	current = command->commands;
	i = 0;
	if (!current)
		return (-1);
	while (i < command->cmd_tracker)
	{
		current = current->next;
		i++;
	}
	return (current->infileindex);
}
void	ft_config(t_command *command)
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
		dup2(current->outfileindex, STDOUT_FILENO);
}

void	pipex(t_command *command)
{
	pid_t		pid;
	int			*fd;

	pid = getpid();
	if (!config_infiles(command))
		return (perror("FILE ERROR"));
	if (!config_outfiles(command))
		return (perror("FILE ERROR"));
	while (command->cmd_tracker < command->cmd_count - 1 && pid != 0)
	{
		fd = create_pipe();
		pid = ft_fork();
		if (!pid)
			ft_childproces(fd, command);
		else
		{
			if (getinf(command) != -1)
			{
				close(fd[1]);
				dup2(getinf(command), STDIN_FILENO);
				close(fd[0]);
			}
			else
			{
				close(fd[1]);
				dup2(fd[0], STDIN_FILENO);
				close(fd[0]);
			}
		}
		command->cmd_tracker++;
		free(fd);
	}
	pid = ft_fork();
	if (!pid)
	{
		ft_config(command);
		ft_execute(command);
		exit(EXIT_SUCCESS);
	}
	close(STDIN_FILENO);
	waitpid(pid, &command->exitstatus, 0);
	while (wait(NULL) != -1)
		;
	exit(command->exitstatus);
}
