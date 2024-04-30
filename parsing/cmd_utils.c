/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cmd_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/22 16:24:09 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/04/30 13:07:51 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void init_struct(t_command *command)
{
	command->infile = NULL;
	command->commands = NULL;
	command->variables = NULL;
	command->outfile = NULL;
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
	
	newtoken = malloc(ft_strlen(token) - j + ft_strlen(value) + 1); //protec
	k = 0;
	while (k < (i - 1))
	{
		newtoken[k] = token[k];
		k++;
	}
	if (value)
		newtoken = ft_strjoin(newtoken, value);
	newtoken = ft_strjoin(newtoken, token + (k + j + 1));
	return (free(token), newtoken);
}

char *ft_expandvariable(t_command *command, char *token, int i)
{
	char *variable;
	char *value;
	int	j;

	j = 0;
	while (token[i + j] != ' ' && token[i + j] != '$' && token[i + j] != '\0' && token[i + j] != '\"' && token[i + j] != '\'') 
		j++;
	variable = malloc(sizeof(char) * (j + 1));
	if (!variable)
		ft_mallocfail(command, "FAIL");
	ft_strlcpy(variable, token + i, j + 1);
	value = getenv(variable);
	free(variable);
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
	// {
	// 	t_list *newnode;
	// 	t_list *lastnode;

	// 	newnode = malloc(sizeof(t_list));
	// 	if (!newnode)
	// 		ft_mallocfail(command, "MALLOC FAIL IN ADDVARIABLE");
	// 	newnode->str = ft_safe_strdup(command->tokens[j], command);
	// 	newnode->next = NULL;
	// 	if (command->variables == NULL)
	// 	{
	// 		command->variables = newnode;
	// 		ft_expandvariable(command, j);
	// 		return;
	// 	}
	// 	lastnode = command->variables;
	// 	while (lastnode->next != NULL)
	// 		lastnode = lastnode->next;
	// 	lastnode->next = newnode;
	// 	ft_expandvariable(command, j);
	// }
}

void	addcommand(t_command *command, int j)
{
	t_list	*newnode;
	t_list	*lastnode;

	command->tokens[j] = ft_strtrim(command->tokens[j], "\""); //nieuwe strtim maken die alleen 1 aanhalingsteken trimt??
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

void init_commands(t_command *command, char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i] != NULL)
	{
		if ((!ft_strncmp(tokens[i], "<", 2) || !ft_strncmp(tokens[i], ">", 2)))
			i++;
		else if (!ft_strncmp(tokens[i], "|", 2))
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
		else if (ft_strncmp(tokens[i], "<", 1) && ft_strncmp(tokens[i], "<<", 2) &&
				 ft_strncmp(tokens[i], ">", 1) && ft_strncmp(tokens[i], ">>", 2))
			addcommand(command, i);
		i++;
	}
}
