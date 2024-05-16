/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/12 16:41:56 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/13 15:58:57 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *ft_findpath(t_command *command, char *cmd, char **envp)
{
	char **paths;
	char *pathpart;
	char *path;
	int i;

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
	int i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i][0] == '\"' && cmd[i][ft_strlen(cmd[i]) - 1] == '\"')
			cmd[i] = ft_strtrim(cmd[i], "\""); // protec + trim maar 1 quote
		else if (cmd[i][0] == '\'' && cmd[i][ft_strlen(cmd[i]) - 1] == '\'')
			cmd[i] = ft_strtrim(cmd[i], "\'");
		i++;
	}
	return (cmd);
}

void ft_execute(t_command *command)
{
	char **cmd_split;
	char *path;
	char *cmd;

	cmd = getcommand(command);
	if (built_in(command, cmd) > 0)
		return;
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
}

int ft_open(char **files, int mode)
{
	int i;
	int fd;

	i = 0;
	while (files && files[i])
	{
		if (!mode)
		{
			fd = open(files[i], O_RDONLY, 0777);
			if (fd == -1)
				return (1);
			close(fd);
		}
		else
		{
			fd = open(files[i], O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (fd == -1)
				return (1);
			close(fd);
		}
		i++;
	}
	return (0);
}

int config_infiles(t_command *command)
{
	t_list *current;
	int i;
	int fd;

	current = command->commands;
	i = 0;
	if (ft_open(command->infiles, 0))
		return (0);
	while (current != NULL)
	{
		if (current->infileindex != -1)
		{
			current->infileindex = open(command->infiles[i], O_RDONLY, 0777);
			if (current->infileindex == -1)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int config_outfiles(t_command *command)
{
	t_list *current;
	int i;
	int fd;

	current = command->commands;
	i = 0;
	if (ft_open(command->outfiles, 1))
		return (0);
	while (current != NULL)
	{
		if (current->outfileindex != -1)
		{
			current->outfileindex = open(command->outfiles[i], O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (current->outfileindex == -1)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int getinf(t_command *command)
{
	t_list *current;
	int i;

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

void ft_config(t_command *command)
{
	t_list *current;
	int i;

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

void ft_childproces(int fd[2], t_command *command)
{
	t_list *current;
	int i;

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

void single_cmd(t_command *command)
{
	t_list *current;
	pid_t pid;

	pid = ft_fork();
	current = command->commands;
	if (!pid)
	{
		if (current->infileindex > 2)
			dup2(current->infileindex, STDIN_FILENO);
		if (current->outfileindex > 2)
			dup2(current->outfileindex, STDOUT_FILENO);
		ft_execute(command);
	}
	else
	{
		waitpid(pid, NULL, 0);
		if (current->infileindex > 2)
			close(current->infileindex);
		if (current->outfileindex > 2)
			close(current->outfileindex);
	}
}

void ft_checkinfile(t_command *command, int fd[2])
{
	t_list *current;
	int i;

	current = command->commands;
	if (!current)
		return;
	i = 0;
	while (i < command->cmd_tracker)
	{
		current = current->next;
		i++;
	}
	if (current->infileindex > 2)
		dup2(current->infileindex, STDIN_FILENO);
	else
		dup2(fd[0], STDIN_FILENO);
}

void pipex(t_command *command)
{
	pid_t pid;
	int *fd;

	pid = getpid();
	if (!config_infiles(command))
		return (perror("FILE ERROR(1)"));
	if (!config_outfiles(command))
		return (perror("FILE ERROR(0)"));
	if (command->cmd_count == 1)
		return (single_cmd(command));
	while (command->cmd_tracker < command->cmd_count - 1 && pid != 0)
	{
		fd = create_pipe();
		pid = ft_fork();
		if (!pid)
		{
			ft_checkinfile(command, fd);
			ft_childproces(fd, command);
		}
		else
		{
			if (getinf(command) > 2 || getinf(command) == -1)
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
			command->cmd_tracker++;
			free(fd);
		}
		waitpid(pid, &command->exitstatus, 0);
		close(STDIN_FILENO);
		// close(STDOUT_FILENO);
		while (wait(NULL) != -1)
			;
		exit(command->exitstatus);
	}
}
