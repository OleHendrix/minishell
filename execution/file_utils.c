/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   file_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/14 15:45:44 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/22 16:34:23 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	ft_handledelimiter(t_command *command, t_list *current)
{
	char	*input;
	char	*str;
	int		*fd;
	
	str = get_next_line(STDIN_FILENO);
	fd = create_pipe(command);
	input = NULL;
	while (ft_strncmp(str, current->cmd_delimiter, ft_strlen(str) - 1))
	{
		input = ft_strjoin(input, str);
		free(str);
		str = get_next_line(STDIN_FILENO);
	}
	free(str);
	write(fd[1], input, ft_strlen(input));
	if (dup2(fd[0], STDIN_FILENO) == -1)
			ft_exit(command, "ERROR IN DUP2");
	free(input);
	close(fd[0]);
	close(fd[1]);
	free(fd);
	free(current->cmd_delimiter);
	current->cmd_delimiter = NULL;
}

int config_infiles(t_command *command, bool next_cmd)
{
	t_list	*current;
	int		fd2;
	
	current = getcommand_node(command);
	if (!current)
		return (0);
	printf("%d %d\n", command->infiletracker, current->infileindex);
	if (command->cmd_tracker > 0 && command->cmd_count > 0 && next_cmd && current->next)
		current = current->next;
	if (current->cmd_delimiter)
		ft_handledelimiter(command, current);
	while (command->infiletracker < current->infileindex)
	{
		fd2 = open(command->infiles[command->infiletracker], O_RDONLY, 0777);
		command->infiletracker ++;
		if (fd2 == -1)
			return (perror("FILE ERROR1"), 0);
		close(fd2);
	}
	if (current->infileindex > -1)
	{
		current->infile_fd = open(command->infiles[current->infileindex], O_RDONLY, 0777);
		command->infiletracker ++;
		if (current->infile_fd == -1)
			return (perror("FILE ERROR"), 0);
		else if (dup2(current->infile_fd, STDIN_FILENO) == -1)
			return (0);
		return (2);
	}
	return (1);
}

void	ft_outfilemode(t_command *command, t_list *current)
{
	int	i;

	i = 0;
	while (command->outfappend[i] != -1)
	{
		if (command->outfappend[i] == current->outfileindex)
		{
			current->outfile_fd = open(command->outfiles[current->outfileindex], O_WRONLY | O_CREAT | O_APPEND, 0777);
			break ;
		}
		i++;
	}
	if (command->outfappend[i] == -1)
		current->outfile_fd = open(command->outfiles[current->outfileindex], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (dup2(current->outfile_fd, STDOUT_FILENO) == -1)
		ft_mallocfail(command, "FAIL");
}

int config_outfiles(t_command *command)
{
	t_list	*current;
	int		fd2;

	current = getcommand_node(command);
	if (!current)
		return (0);
	while (command->outfiletracker < current->outfileindex)
	{
		fd2 = open(command->outfiles[command->outfiletracker], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fd2 == -1)
			return (perror("FILE ERROR2"), 0);
		close(fd2);
		command->outfiletracker ++;
	}
	if (current->outfileindex > -1)
	{
		ft_outfilemode(command, current);
		// current->outfile_fd = open(command->outfiles[current->outfileindex], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		// if (current->outfile_fd == -1)
		// 	return (perror("FILE ERROR"), 0);
		// if (dup2(current->outfile_fd, STDOUT_FILENO) == -1)
		// 	return (0);
		command->outfiletracker ++;
		return (2);
	}
	return (1);
}
