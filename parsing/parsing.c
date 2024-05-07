/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 11:02:47 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/07 17:12:43 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_files_to_com(t_command *command)
{
	t_list *current;
	int		i;

	current = command->commands;
	if (!current)
		return;
	printf("yes\n");
	i = 0;
	while (current->next != NULL)
		current = current->next;
	printf("%s: outfileindex: %d\n", current->str, current->outfileindex);
	if (!command->infilereset)
	{
		if (!command->infiles[i])
			current->infileindex = -1;
		else
		{
			while (command->infiles[i] != NULL)
				i++;
			current->infileindex = i - 1;
		}
	}
	i = 0;
	if (!command->outfilereset)
	{
		if (!command->outfiles[i])
			current->outfileindex = -1;
		else
		{
			while (command->outfiles[i] != NULL)
			{
				printf("sii: %s\n", current->str);
				i++;
			}
			current->outfileindex = i - 1;
		}
	}
	printf("%s: outfileindex: %d\n", current->str, current->outfileindex);
	// if (command->infiles[i] == NULL)
	// 	current->infile = NULL;
	// else
	// {
	// 	while (command->infiles[i] != NULL)
	// 		i++;
	// 	current->infile = ft_safe_strdup(command->infiles[i - 1], command);
	// 	// printf("%s\n", current->infile);
	// }
	// i = 0;
	// if (command->outfiles[i] == NULL)
	// 	current->outfile = NULL;
	// else
	// {
	// 	while (command->outfiles[i] != NULL)
	// 		i++;
	// 	current->outfile = ft_safe_strdup(command->outfiles[i - 1], command);
	// }
}

void	addoutfile(t_command *command, char *str)
{
	char **newarray;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (command->outfiles[count])
		count++;
	newarray = malloc((count + 2) * sizeof(char *));
	if (!newarray)
		ft_mallocfail(command, "FAIL");
	while (i < count)
	{
		newarray[i] = ft_safe_strdup(command->outfiles[i], command);
		i++;
	}
	newarray[i] = ft_safe_strdup(str, command);
	newarray[count + 1] = NULL;
	free_ptr_ptr(command->outfiles);
	command->outfiles = newarray;
}

void	addinfile(t_command *command, char *str)
{
	char **newarray;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (command->infiles[count])
		count++;
	newarray = malloc((count + 2) * sizeof(char *));
	if (!newarray)
		ft_mallocfail(command, "FAIL");
	while (i < count)
	{
		newarray[i] = ft_safe_strdup(command->infiles[i], command);
		i++;
	}
	newarray[i] = ft_safe_strdup(str, command);
	newarray[count + 1] = NULL;
	free_ptr_ptr(command->infiles);
	command->infiles = newarray;
}

int	ft_set_files(t_command *command, char *str, int set)
{
	if (set == 0) //infile
	{
		addinfile(command, str);
		command->infilereset = false;
	}
	if (set == 1) //outfile
	{
		addoutfile(command, str);
		command->outfilereset = false;
	}
	return (1);
	
	// if (set == 0)
	// {
	// 	if (command->infile)
	// 		free(command->infile);
	// 	command->infile = ft_safe_strdup(str, command);
	// }
	// if (set == 1)
	// {
	// 	if (command->outfile)
	// 		free(command->outfile);
	// 	command->outfile = ft_safe_strdup(str, command);
	// }
	// return (1);
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
	ft_files_to_com(command);
}

int	init_commands(t_command *command, char **tokens, int i)
{
	if (!ft_strncmp(tokens[i], "|", 2))
	{
		command->pipes++;
		command->newcom = true;
		command->infilereset = true;
		command->outfilereset = true;
		ft_files_to_com(command);
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
			current->str = ft_safe_strjoin(command, current->str, current->next->str);
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

void	fill_struct(char *line, t_command *command, char *mode)
{
	init_struct(command);
	if (built_in_perm(command, line))
		return ;
	ft_supersplit(line, ' ', command);
	if (!command->tokens)
		return (ft_putstr_fd("ERROR IN SPLIT", 2));
	init_files(command, command->tokens);
	command->cmd_count = count_commands(&command->commands);
	// if (mode && !ft_strncmp(mode, "test", 5))
	// 	printstruct(command);
	// else
	// {
	// 	command->pid = fork();
	// 	if (!command->pid)
	// 		pipex(command);
	// 	else
	// 		waitpid(command->pid, &command->exitstatus, 0);

	// }
	ft_free_struct(command);
}

// void	print_list(char **str)
// {
// 	int		i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		printf("%s\n", str[i]);
// 		i ++;
// 	}
// }