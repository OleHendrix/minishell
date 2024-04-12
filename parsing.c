#include "minishell.h"

void printstruct(t_command *command)
{
	int	i;

	i = 0;
	if (command->pipe)
		printf("pipe: true\n");
	if (command->infile)
		printf("infile: %s\n", command->infile);
	if (command->outfile)
		printf("outfile: %s\n", command->outfile);
	if (command->commands)
		printf("commands:\n");
	while (command->commands != NULL && command->commands[i] != NULL)
	{
		printf("%s\n", command->commands[i]);
		i++;
	}
}

void init_command(t_command *command)
{
	command->infile = NULL;
	command->commands = NULL;
	command->outfile = NULL;
	command->here_doc = '\0';
	command->delimiter = NULL;
	command->pipe = false;
}

int	ft_checkflags(t_command *command, int i, char **tokens, int j)
{
	if (tokens[j + 1] == NULL || !ft_strncmp(tokens[j + 1], "|", 2) || !ft_strncmp(tokens[j + 1], ">", 2))
	{
		if (j > 0 && ft_strncmp(tokens[j - 1], "|", 2))
		{
			command->commands[i] = ft_strjoin(command->commands[i], " ");
			command->commands[i] = ft_strjoin(command->commands[i], tokens[j]);
			return (1);
		}
	}
	return (0);
}

void	addcommand(t_command *command, char **tokens, int j)
{
	int	i;
	char **commands2;

	i = 0;
	while (command->commands && command->commands[i] != NULL)
		i++;
	if (command->commands)
	{
		if (ft_checkflags(command, i - 1, tokens, j))
			return ;
	}
	commands2 = realloc(command->commands, sizeof(char *) * (i + 2));
	if (!commands2)
		return ;
	commands2[i] = ft_strdup(tokens[j]);
	commands2[i + 1] = NULL; 
	command->commands = commands2;
}

void init_files(t_command *command, char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i] != NULL)
	{
		if (!ft_strncmp(tokens[i], "<", 2))
		{
			command->infile = ft_strdup(tokens[i + 1]);
			i++;
		}
		else if (!ft_strncmp(tokens[i], ">", 2))
		{
			command->outfile = ft_strdup(tokens[i + 1]);
			i++;
		}
		i++;
	}
}

void	init_commands(t_command *command, char **tokens)
{
	int i;

	i = 0;
	while (tokens[i] != NULL)
	{
		if (!ft_strncmp(tokens[i], "<", 2) || !ft_strncmp(tokens[i], ">", 2))
			i += 2;
		else if (!ft_strncmp(tokens[i], "|", 2))
		{
			command->pipe = true;
			i++;
		}
		else
		{
			addcommand(command, tokens, i);
			i++;
		}
	}
}


void fill_struct(char *line, char **envp)
{
	t_command	command;
	int			i;
	char		**tokens;

	init_command(&command);
	i = 0;
	tokens = ft_split(line, ' ');
	init_files(&command, tokens);
	init_commands(&command, tokens);
	printstruct(&command);
}

void	ft_parsing(char *line, char **envp)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
		return (perror("Fork failed"));
	if (pid == 0)
	{
		// check_input(line);
		fill_struct(line, envp);
	}
	else
		waitpid(pid, NULL, 0);
	
}