/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing_files.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/14 15:54:07 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/22 16:00:36 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_files_to_com2(t_command *command, t_list *current)
{
	int	i;

	i = 0;
	if (!command->outfilereset)
	{
		if (!command->outfiles[i])
			current->outfileindex = -1;
		else
		{
			while (command->outfiles[i] != NULL)
				i++;
			current->outfileindex = i - 1;
		}
	}
	current->filesset = true;
}

void ft_files_to_com(t_command *command)
{
	t_list *current;
	int		i;

	current = command->commands;
	if (!current)
		return;
	i = 0;
	while (current->next != NULL)
		current = current->next;
	if (current->filesset)
		return;
	if (command->here_doc)
	{
		current->cmd_delimiter = ft_safe_strdup(command->delimiter, command);
		command->here_doc = false;
		free(command->delimiter);
	}
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
	ft_files_to_com2(command, current);
}

int	addoutfile(t_command *command, char *str)
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
	return (i);
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

void	appendindex(int x, t_command *command)
{
	int	i;
	int	j;
	int	*outfappend;
	
	i = 0;
	j = 0;
	while (command->outfappend[i] != -1)
		i ++;
	outfappend = malloc((i + 2) * sizeof (int));
	if (!outfappend)
		ft_mallocfail(command, "FAIL");
	while (command->outfappend[j] != -1)
	{
		outfappend[j] = command->outfappend[j];
		j ++;
	}
	outfappend[j] = x;
	outfappend[j + 1] = -1;
	free(command->outfappend);
	command->outfappend = outfappend;
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
	if (set == 2) //delim
	{
		appendindex(addoutfile(command, str), command);
		command->outfilereset = false;
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
		else if (!ft_strncmp(tok[i], ">", 2) && ft_strncmp(tok[i], ">>", 2))
		{
			i += ft_set_files(command, tok[i + 1], 1);
			i ++;
		}
		else if (tok[i][0] == '<' && tok[i][1] != '<' && tok[i][2] != '\0')
			i += ft_set_files(command, &tok[i][1], 0);
		else if (tok[i][0] == '>' && tok[i][1] != '>' && tok[i][2] != '\0')
			i += ft_set_files(command, &tok[i][1], 1);
		else if (!ft_strncmp(tok[i], ">>", 2) && tok[i][2] != '\0')
			i += ft_set_files(command, &tok[i][2], 2);
		else if (!ft_strncmp(tok[i], ">>", 3))
		{
			i += ft_set_files(command, tok[i + 1], 2);
			i ++;
		}	
		else
			i = init_commands(command, command->tokens, i);
	}
	ft_files_to_com(command);
}
