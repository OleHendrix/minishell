/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cmd_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/22 16:24:09 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/07 16:56:11 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void init_struct(t_command *command)
{
	command->infile = NULL;
	command->infiles = malloc(1);
	if (!command->infiles)
		ft_mallocfail(command, "FAIL");
	command->infiles[0] = NULL;
	command->infile_fd = 0;
	command->outfile = NULL;
	command->outfiles = malloc(1);
	if (!command->outfiles)
		ft_mallocfail(command, "FAIL");
	command->outfiles[0] = NULL;
	command->outfile_fd = 0;
	command->commands = NULL;
	command->here_doc = false;
	command->delimiter = NULL;
	command->cmd_count = 0;
	command->cmd_tracker = 0;
	command->pipes = 0;
	command->newcom = false;
	command->infilereset = false;
	command->outfilereset = false;
}

char *ft_append(t_command *command, int i, int j, int t_idx)
{
	char *newtoken;
	int	k;

	newtoken = ft_calloc(sizeof(char), ft_strlen(command->tokens[t_idx]) + ft_strlen(command->value) - j + 2);
	if (newtoken == NULL)
		ft_mallocfail(command, "MALLOC FAILED IN APPEND");
	k = 0;
	while (k < i - 1)
	{
		newtoken[k] = command->tokens[t_idx][k];
		k++;
	}
	if (command->value)
		newtoken = ft_safe_strjoin(command, newtoken, command->value);
	newtoken = ft_safe_strjoin(command, newtoken, command->tokens[t_idx] + (i + j));
	return (free(command->tokens[t_idx]), newtoken);
}


char *ft_expandvariable(t_command *command, int i, int t_idx)
{
	char *variable;
	int	j;

	j = 0;
	if (command->tokens[t_idx][i] == '?')
	{
		j = 1;
		command->value = ft_itoa(command->exitstatus);
	}
	else
	{
		j = ft_stritr(command->tokens[t_idx] +i + j, " $\'\"\0");
		if (j == 0)
			return (command->tokens[t_idx]);
		variable = ft_substr(command->tokens[t_idx], i, j);
		if (!variable)
			ft_mallocfail(command, "FAIL");
		command->value = ft_getenv(command, variable);
		free(variable);
	}
	return (ft_append(command, i, j, t_idx));
}

void	ft_variable(t_command *command, int j)
{
	int	i;

	i = 0;
	command->inquotes = false;
	while (command->tokens[j][i] != '\0')
	{
		if (command->tokens[j][i] == '\'')
			command->inquotes = !command->inquotes;
		if (command->tokens[j][i] == '$' && !command->inquotes)
		{
			command->tokens[j] = ft_expandvariable(command, i + 1, j);
			if (command->tokens[j][0] == '\0')
				break;
		}
		i++;
	}
	command->inquotes = false;
}

char *ft_strtrim2(char *str, char c)
{
	size_t	i;
	char *ret;

	i = 1;
	if (str[0] != c || str[ft_strlen(str) - 1] != c)
	{
		return (str);
	}
	ret = (char *)malloc (sizeof(char *) * ft_strlen(str) - 1);
	while (i < ft_strlen(str) - 1)
	{
		ret[i - 1] = str[i];
		i ++;
	}
	free(str);
	return (ret);
}

void	addcommand(t_command *command, int j)
{
	t_list	*newnode;
	t_list	*lastnode;

	command->tokens[j] = ft_strtrim2(command->tokens[j], '\"');
	ft_variable(command, j);
	if (!command->newcom || command->tokens[j][0] == '\0')
		return (ft_checkflags(command, j)) ;
	newnode = malloc(sizeof(t_list));
	if (!newnode)
		ft_mallocfail(command, "MALLOC FAILED IN ADDCOMMAND");
	newnode->str = ft_safe_strdup(command->tokens[j], command);
	newnode->infileindex = -1;
	newnode->outfileindex = -1;
	newnode->next = NULL;
	if (command->commands == NULL)
	{
		command->commands = newnode;
		return;
	}
	lastnode = command->commands;
	while (lastnode->next != NULL)
		lastnode = lastnode->next;
	lastnode->next = newnode;
	command->newcom = false;
}

