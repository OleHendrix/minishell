/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cmd_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/22 16:24:09 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/03 16:45:30 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void init_struct(t_command *command)
{
	command->infile = NULL;
	command->infile_fd = 0;
	command->outfile = NULL;
	command->outfile_fd = 0;
	command->commands = NULL;
	command->variables = NULL;
	command->here_doc = false;
	command->delimiter = NULL;
	command->cmd_count = 0;
	command->cmd_tracker = 0;
	command->pipes = 0;
	command->newcom = false;
}

char *ft_append(char *token, char *value, int i, int j)
{
	char *newtoken;
	int	k;
	
	newtoken = malloc(ft_strlen(token) - (j + 1) + ft_strlen(value) + 1); //protec
	ft_bzero(newtoken, ft_strlen(token) - (j + 1) + ft_strlen(value) + 1);
	k = 0;
	while (k < i - 1)
	{
		newtoken[k] = token[k];
		k++;
	}
	if (value)
	{
		// printf("%s\n", value);
		newtoken = ft_strjoin(newtoken, value);
		// printf("%s\n", newtoken);
	}
	printf("%s\n", newtoken);
	newtoken = ft_strjoin(newtoken, token + (i + j));		//gaat fout met echo fobofe $US (hij haalt eerste letter weg van vorige woord)
	return (free(token), newtoken);
}

char *ft_expandvariable(t_command *command, char *token, int i)
{
	char *variable;
	char *value;
	int	j;

	j = 0;
	if (token[i] == '?')
	{
		j = 1;
		value = ft_itoa(command->exitstatus);
	}
	else
	{
		while (token[i + j] != ' ' && token[i + j] != '$' && token[i + j] != '\0' && token[i + j] != '\"' && token[i + j] != '\'') 
			j++;
		if (j == 0)
			return (token);
		variable = malloc(sizeof(char) * (j + 1));
		if (!variable)
			ft_mallocfail(command, "FAIL");
		ft_strlcpy(variable, token + i, j + 1);
		value = ft_getenv(command, variable); //zelf maken 
		free(variable);
	}
	return (ft_append(token, value, i, j));
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
			command->tokens[j] = ft_expandvariable(command, command->tokens[j], i + 1);
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
	if (!command->newcom)
	{
		ft_checkflags(command, j);
		return ;
	}
	newnode = malloc(sizeof(t_list));
	if (!newnode)
		ft_mallocfail(command, "MALLOC FAILED IN ADDCOMMAND");
	newnode->str = ft_safe_strdup(command->tokens[j], command);
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

