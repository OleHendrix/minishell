#include "minishell.h"

void	ft_free(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

// void printcommand(char **argv)
// {
// 	int	i;
// 	int j;

// 	i = 0;
// 	while (i < 4)
// 	{
// 		printf("%s\n", argv[i]);
// 		i++;
// 	}
// }

// void	check_input(char *line)
// {
// 	int	quotes;
// 	int quotesdouble;
// 	int	i;

// 	quotes = 0;
// 	quotesdouble = 0;
// 	i = 0;
// 	while (line[i] == ' ')
// 		i++;
// 	if (line[i] == '<' || line[i] == '>' || line[ft_strlen(line) - 1] == '<' || line[ft_strlen(line) - 1] == '>')
// 		return (perror("Wrong syntax"));
// 	while (line[i] != '\0')
// 	{
// 		if (line[i] == '\'')
// 			quotes++;
// 		if (line[i] == '\"')
// 			quotesdouble++;
// 		i++;
// 		if (line[i] == '>' && (line[i + 1] == '\0' || line[i + 1] == '>'))
// 			return (perror("Wrong syntax"));
// 	}
// 	if (quotes % 2 == 1 || quotesdouble % 2 == 1)
// 		return (perror("Wrong syntax"));
// }

void init_command(t_command *command)
{
	command->infile = NULL;
	command->commands = NULL;
	command->outfile = NULL;
	command->here_doc = '\0';
	command->delimiter = NULL;
}

char **addcommand(t_command *command, char *token)
{
	int	i;
	char **commands2;

	i = 0;
	while (command->commands && command->commands[i] != NULL)
		i++;
	commands2 = realloc(command->commands, sizeof(char *) * (i + 2));
	if (!commands2)
		return (NULL);
	commands2[i] = ft_strdup(token);
	commands2[i + 1] = NULL; 
	command->commands = commands2;
	return (commands2);
}
void printstruct(t_command *command)
{
	int	i;

	i = 0;
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

void fill_struct(char *line, char **envp)
{
	t_command	command;
	int			i;
	char		**tokens;

	init_command(&command);
	i = 0;
	tokens = ft_split(line, ' ');
	while (tokens[i] != NULL)
	{
		if (!ft_strncmp(tokens[i], "<", 2))
		{
			command.infile = ft_strdup(tokens[i + 1]);
			i++;
		}
		else if (!ft_strncmp(tokens[i], ">", 2))
		{
			command.outfile = ft_strdup(tokens[i + 1]);
			i++;
		}
		else
			addcommand(&command, tokens[i]);
		i++;
	}
	printstruct(&command);
}

// void	parsing2(char *line, char **envp)
// {
// 	pid_t pid;

// 	pid = fork();
// 	if (pid == -1)
// 		return (perror("Fork failed"));
// 	if (pid == 0)
// 	{
// 		// check_input(line);
// 		fill_struct(line, envp);
// 	}
// 	waitpid(pid, NULL, 0);
// }

// void	parsing(char *line, char **envp)
// {
// 	char **tokens;
// 	char **argv;
// 	int i;

// 	i = 2;
// 	tokens =  ft_split(line, ' ');
// 	argv = malloc(4 * sizeof(char *));
// 	argv[0] = ft_strjoin(argv[0], tokens[1]);
// 	while (tokens[i][0] != '|')
// 	{
// 		argv[1] = ft_strjoin(argv[1], tokens[i]);
// 		i++;
// 		if (tokens[i][0] != '|')
// 			argv[1] = ft_strjoin(argv[1], " ");
// 	}
// 	i++;
// 	while (tokens[i][0] != '>')
// 	{
// 		argv[2] = ft_strjoin(argv[2], tokens[i]);
// 		i++;
// 		if (tokens[i][0] != '>')
// 			argv[2] = ft_strjoin(argv[2], " ");
// 	}
// 	argv[3] = ft_strjoin(argv[3], tokens[i + 1]);
// 	pipex(argv, envp);
// }

void parse_input(char **envp)
{
	char *line;

	while (1)
	{
		line = readline("minishell: ");
		if (!ft_strncmp(line, "exit", 4))
			break ;
		fill_struct(line, envp);
	}
}

int main(int argc, char **hallo, char **envp)
{
	if (!envp || argc < 1)
		return (1);
	parse_input(envp);
}