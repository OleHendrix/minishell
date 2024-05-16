void	ft_set_files(t_command *command, char *str, int i)
{
	if (i == 0)
	{
		if (command->infile)
			free(command->infile);
		command->infile = ft_safe_strdup(str, command);
	}
	if (i == 1)
	{
		if (command->outfile)
			free(command->outfile);
		command->outfile = ft_safe_strdup(str, command);
	}
}

void init_files(t_command *command, char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i] != NULL)
	{
		if (!ft_strncmp(tokens[i], "<", 1) && ft_strncmp(tokens[i], "<<", 2) )
			ft_set_files(command, &tokens[i][1], 0);
		if (!ft_strncmp(tokens[i], "<", 2))
		{
			ft_set_files(command, tokens[i + 1], 0);
			i++;
		}
		if (!ft_strncmp(tokens[i], ">", 1) && ft_strncmp(tokens[i], ">>", 2) )
			ft_set_files(command, &tokens[i][1], 1);
		if (!ft_strncmp(tokens[i], ">>", 2) )
			ft_set_files(command, &tokens[i][2], 1);
		if (!ft_strncmp(tokens[i], ">", 2) || !ft_strncmp(tokens[i], ">>", 3))
		{
			ft_set_files(command, tokens[i + 1], 1);
			i++;
		}
		i++;
	}
}

void init_commands(t_command *command, char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i] != NULL)
	{
		if ((!ft_strncmp(tokens[i], "<", 2) || !ft_strncmp(tokens[i], ">", 2) || !ft_strncmp(tokens[i], "<<", 3) || !ft_strncmp(tokens[i], ">>", 3)))
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


// char *ft_expandvariable(t_command *command, char *token, int i, int token_index)
// {
// 	char *variable;
// 	char *value;
// 	int	j;

// 	j = 0;
// 	if (token[i] == '?')
// 	{
// 		j = 1;
// 		value = ft_itoa(command->exitstatus);
// 	}
// 	else
// 	{
// 		while (token[i + j] != ' ' && token[i + j] != '$' && token[i + j] != '\0' && token[i + j] != '\"' && token[i + j] != '\'') 
// 			j++;
// 		if (j == 0)
// 			return (token);
// 		variable = ft_substr(token, i, j);
// 		if (!variable)
// 			ft_mallocfail(command, "FAIL");
// 		value = ft_getenv(command, variable);
// 		free(variable);
// 	}
// 	return (ft_append(token, value, i, j));
// }