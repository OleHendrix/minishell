// #include "minishell.h"

// void printstruct(t_command *command)
// {
// 	int	i;

// 	i = 0;
// 	if (command->pipe)
// 		printf("pipe: true\n");
// 	if (command->infile)
// 		printf("infile: %s\n", command->infile);
// 	if (command->outfile)
// 		printf("outfile: %s\n", command->outfile);
// 	if (command->here_doc)
// 		printf("here doc: true\n");
// 	if (command->delimiter)
// 		printf("delimiter: %s\n", command->delimiter);
// 	if (command->commands)
// 		printf("commands:\n");
// 	while (command->commands != NULL && command->commands[i] != NULL)
// 	{
// 		printf("%s\n", command->commands[i]);
// 		i++;
// 	}
// }

// void init_command(t_command *command)
// {
// 	command->infile = NULL;
// 	command->commands = NULL;
// 	command->outfile = NULL;
// 	command->here_doc = false;
// 	command->delimiter = NULL;
// 	command->pipe = false;
// }

// int ft_flags(char **tokens, int j)
// {
// 	if (tokens[j + 1] == NULL)
// 		return (1);
// 	if (!ft_strncmp(tokens[j + 1], "|", 2))
// 		return (1);
// 	if (!ft_strncmp(tokens[j + 1], ">", 2))
// 		return (1);
// 	if (tokens[j][0] == '\"' || tokens[j][ft_strlen(tokens[j]) - 1] == '\"')
// 		return (1);
// 	return (0);
// }

// int	ft_checkflags(t_command *command, int i, char **tokens, int j)
// {
// 	if (ft_flags(tokens, j))
// 	{
// 		if (j > 0 && ft_strncmp(tokens[j - 1], "|", 2))
// 		{
// 			if (tokens[j][0] == '\"')|| tokens[j][ft_strlen(tokens[j]) - 1] == '\"')
// 				tokens[j] = ft_strtrim(tokens[j], "\"");
// 			command->commands[i] = ft_strjoin(command->commands[i], " ");
// 			command->commands[i] = ft_strjoin(command->commands[i], tokens[j]);
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

// void	addcommand(t_command *command, char **tokens, int j)
// {
// 	int	i;
// 	char **commands2;

// 	i = 0;
// 	while (command->commands && command->commands[i] != NULL)
// 		i++;
// 	if (command->commands)
// 	{
// 		if (ft_checkflags(command, i - 1, tokens, j))
// 			return ;
// 	}
// 	commands2 = realloc(command->commands, sizeof(char *) * (i + 2));
// 	if (!commands2)
// 		return ;
// 	commands2[i] = ft_strdup(tokens[j]);
// 	commands2[i + 1] = NULL; 
// 	command->commands = commands2;
// }

// void init_files(t_command *command, char **tokens)
// {
// 	int	i;

// 	i = 0;
// 	while (tokens[i] != NULL)
// 	{
// 		if (!ft_strncmp(tokens[i], "<", 2))
// 		{
// 			command->infile = ft_strdup(tokens[i + 1]);
// 			i++;
// 		}
// 		else if (!ft_strncmp(tokens[i], ">", 2))
// 		{
// 			command->outfile = ft_strdup(tokens[i + 1]);
// 			i++;
// 		}
// 		i++;
// 	}
// }

// void	init_commands(t_command *command, char **tokens)
// {
// 	int i;

// 	i = 0;
// 	while (tokens[i] != NULL)
// 	{
// 		if (!ft_strncmp(tokens[i], "<", 2) || !ft_strncmp(tokens[i], ">", 2))
// 			i += 2;
// 		else if (!ft_strncmp(tokens[i], "|", 2))
// 		{
// 			command->pipe = true;
// 			i++;
// 		}
// 		else if (!ft_strncmp(tokens[i], "<<", 3))
// 		{
// 			command->here_doc = true;
// 			command->delimiter = ft_strdup(tokens[i + 1]);
// 			i += 2;
// 		}
// 		else
// 		{
// 			addcommand(command, tokens, i);
// 			i++;
// 		}
// 	}
// }

// // char **cat_tokens(char **tokens)
// // {
// // 	int i;

// // 	i = 0;
// // 	while (tokens[i])
// // 	{
// // 		if (tokens[i][0] == '\"')
// // 		{
// // 			tokens[i] = ft_strjoin(tokens[i], tokens[i + 1]);
// // 			if (tokens[i][ft_strlen(tokens[i]) - 1] != '\"')
// // 				i ++;
// // 			else 
// // 				break;
// // 		}
// // 	}
// // 	return (NULL);
// // }
// // char **ft_copytokens(char **tokens, char **tokens2, int i, int j)
// // {
// // 	int	k;
// // 	int l;

// // 	k = 0;
// // 	while (tokens[k][0] != '\"' && tokens[k] != NULL)
// // 	{
// // 		tokens2[k] = ft_strdup(tokens[k]);
// // 		k++;
// // 	}
// // 	l = k;
// // 	while (tokens[k] != NULL)
// // 	{
// // 		tokens2[l] = ft_strjoin(tokens2[l], tokens[k]);
// // 		if (tokens[k][ft_strlen(tokens[k + 1]) - 1] != '\"')
// // 			tokens2[l] = ft_strjoin(tokens2[l], " ");
// // 		if (tokens[k][ft_strlen(tokens[k]) - 1] == '\"')
// // 			break;
// // 		k++;
// // 	}
// // 	k++;
// // 	l++;
// // 	while (tokens[k] != NULL)
// // 	{
// // 		tokens2[l] = ft_strdup(tokens[k]);
// // 		l++;
// // 		k++;
// // 	}
// // 	tokens2[l] = NULL; 
// // 	return (tokens2);
// // }

// // char **addquotes(char **tokens)
// // {
// // 	int	i;
// // 	int	j;
// // 	int count;
// // 	char **tokens2;

// // 	i = 0;
// // 	j = 0;
// // 	count = 0;
// // 	while (tokens[count] != NULL)
// // 		count++;
// // 	while (tokens[i][0] != '\"' && tokens[i] != NULL)
// // 		i++;
// // 	while (tokens[j][ft_strlen(tokens[j]) - 1] != '\"' && tokens[j] != NULL)
// // 		j++;
// // 	tokens2 = malloc(sizeof(char *) * (count - (j - i) + 1));
// // 	return (ft_copytokens(tokens, tokens2, i, j));
// // }

// void fill_struct(char *line, char **envp)
// {
// 	t_command	command;
// 	int			i;
// 	char		**tokens;

// 	init_command(&command);
// 	i = 0;
// 	tokens = ft_split(line, ' ');
// 	tokens = addquotes(tokens);
// 	while (tokens[i] != NULL)
// 	{
// 		printf("%s\n", tokens[i]);
// 		i++;
// 	}



// 	// init_files(&command, tokens);
// 	// init_commands(&command, tokens);
// 	// pipex(&command, envp);
// 	// printstruct(&command);
// }

// void	ft_parsing(char *line, char **envp)
// {
// 	fill_struct2(line, envp);
// }