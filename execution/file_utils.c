/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   file_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/14 15:45:44 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/21 17:08:32 by ohendrix      ########   odam.nl         */
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

	str = get_next_line(STDIN_FILENO);
	// // str = NULL;
	// printf("%d\n", ft_strncmp(str, current->cmd_delimiter, ft_strlen(str - 1)));
	// printf("%s", current->cmd_delimiter);
	// printf("%s", str - 1);
	// while (!ft_strncmp(str, current->cmd_delimiter, ft_strlen(str - 1)))
	// {
	// 	// if (ft_strncmp(str, current->cmd_delimiter, ft_strlen(str - 1)))
	// 	// 	break;
	// 	input = ft_strjoin(input, str);
	// 	str = get_next_line(STDIN_FILENO);
	// }
	// printf("hello\n");
	printf("new :%s", str);
}

int config_infiles(t_command *command, bool next_cmd)
{
	t_list	*current;
	int		fd2;

	current = getcommand_node(command);
	if (!current)
		return (0);
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

int config_outfiles(t_command *command)
{
	t_list	*current;
	int		fd2;

	current = getcommand_node(command);
	if (!current)
		return (0);
	// printf("%d, %d\n", command->outfiletracker , current->outfileindex);
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
		current->outfile_fd = open(command->outfiles[current->outfileindex], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (current->outfile_fd == -1)
			return (perror("FILE ERROR"), 0);
		if (dup2(current->outfile_fd, STDOUT_FILENO) == -1)
			return (0);
		command->outfiletracker ++;
		return (2);
	}
	return (1);
}
