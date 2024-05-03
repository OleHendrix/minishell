/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 11:02:47 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/03 16:14:57 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_set_files(t_command *command, char *str, int set)
{
	if (set == 0)
	{
		if (command->infile)
			free(command->infile);
		command->infile = ft_safe_strdup(str, command);
	}
	if (set == 1)
	{
		if (command->outfile)
			free(command->outfile);
		command->outfile = ft_safe_strdup(str, command);
	}
	return (1);
}

void init_files(t_command *command, char **tok)
{
	int	i;

	i = 0;
	while (tok[i] != NULL)
	{
		if (!ft_strncmp(tok[i], "<", 2))
		{
			i += ft_set_files(command, tok[i + 1], 0);
			i ++;
		}
		else if (!ft_strncmp(tok[i], ">", 2) || !ft_strncmp(tok[i], ">>", 3))
		{
			i += ft_set_files(command, tok[i + 1], 1);
			i ++;
		}
		else if (tok[i][0] == '<' && tok[i][1] != '<' && tok[i][2] != '\0')
			i += ft_set_files(command, &tok[i][1], 0);
		else if (tok[i][0] == '>' && tok[i][1] != '>' && tok[i][2] != '\0')
			i += ft_set_files(command, &tok[i][1], 1);
		else if (!ft_strncmp(tok[i], ">>", 2) )
			i += ft_set_files(command, &tok[i][2], 1);
		else
			i = init_commands(command, command->tokens, i);
	}
}

int	init_commands(t_command *command, char **tokens, int i)
{
	if (!ft_strncmp(tokens[i], "|", 2))
	{
		command->pipes++;
		command->newcom = true;
	}
	else if (!ft_strncmp(tokens[i], "<<", 3))
	{
		command->here_doc = true;
		command->delimiter = ft_safe_strdup(tokens[i + 1], command);
		i++;
	}
	else if (!ft_strncmp(tokens[i], "<<", 2))
	{
		command->here_doc = true;
		command->delimiter = ft_safe_strdup(&tokens[i][2], command);
	}
	else
		addcommand(command, i);
	return (i + 1);
}

void combine_empty_quote(t_command *command)
{
	t_list *current;

	current = command->commands;

	if (current == NULL)
		return ;
	while (current->next != NULL)
	{
		if (!ft_strncmp(current->str, "\"", 2))
		{
			current->str = ft_strjoin(current->str, current->next->str);
			if (!current->str)
				ft_mallocfail(command, "ERROR IN combine_empty_qoute");
			current->next = current->next->next;
		}
		else
			current = current->next;
	}
}

int	count_commands(t_list **list)
{
	t_list *current;
	int		i;

	i = 0;
	current = *list;
	while (current)
	{
		current = current->next;
		i ++;
	}
	return (i);
}

void	print_list(char **str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		printf("%s\n", str[i]);
		i ++;
	}
}

void fill_struct(char *line, t_command *command, char *mode)
{
	init_struct(command);
	if (built_in_perm(command, line))
		return ;
	ft_supersplit(line, ' ', command);
	// print_list(command->tokens);
	// printf("\n\n");
	if (!command->tokens)
		return (ft_putstr_fd("ERROR IN SPLIT", 2));
	init_files(command, command->tokens);
	// init_commands(command, command->tokens);
	// trim_quotes(&command->commands);
	command->cmd_count = count_commands(&command->commands);
	if (mode && !ft_strncmp(mode, "test", 5))
		printstruct(command);
	else
	{
		command->pid = fork();
		if (!command->pid)
			pipex(command);
		else
		{
			wait(&command->exitstatus);
			waitpid(command->pid, NULL, 0);
		}
	}
	ft_free_struct(command);
	free(line);
}
