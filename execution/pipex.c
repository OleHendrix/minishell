/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/12 16:41:56 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/16 15:32:12 by ohendrix      ########   odam.nl         */
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
		exit(EXIT_SUCCESS);
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

	current = getcommand_node(command);
	if (current->outfileindex != -1)
	{
		close(fd[0]);
		if (dup2(current->outfileindex, STDOUT_FILENO) == -1)
			ft_exit(command, "ERROR IN DUP24");	
		close(fd[1]);
	}
	else if (command->cmd_tracker + 1 < command->cmd_count)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			ft_exit(command, "ERROR IN DUP23");
		close(fd[1]);
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
	}
	close(command->save_std_in);
	close(command->save_std_out);
	ft_execute(command);
}

void	ft_configinput(int fd[2], t_command *command)
{
	t_list	*current;

	current = getcommand_node(command);
	close(fd[1]);
	if (!current->next)
	{
		close(fd[0]);
		return ;
	}
	if (current->next->infileindex != -1)
	{
		if (dup2(current->next->infileindex, STDIN_FILENO) == -1)
			ft_exit(command, "ERROR IN DUP21");
		close(fd[0]);
	}
	else
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
			ft_exit(command, "ERROR IN DUP22");
		close(fd[0]);
	}
}

void	pipex(t_command *command)
{
	pid_t		pid;
	int			*fd;

	pid = getpid();
	if (!config_infiles(command))
		return (perror("FILE ERROR(1)"));
	if (!config_outfiles(command))
		return (perror("FILE ERROR(0)"));
	while (command->cmd_tracker < command->cmd_count && pid != 0)
	{
		fd = create_pipe(command);
		pid = ft_fork(command);
		if (!pid)
			ft_childproces(fd, command);
		else
			ft_configinput(fd, command);
		command->cmd_tracker ++;
		free(fd);
	}
	ft_waitpids(command);
	ft_restore_in_out(command);
}
