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

void printcommand(char **argv)
{
	int	i;
	int j;

	i = 0;
	while (i < 4)
	{
		printf("%s\n", argv[i]);
		i++;
	}
}
void	check_input(char *line)
{
	int	quotes;
	int quotesdouble;
	int	i;

	quotes = 0;
	quotesdouble = 0;
	i = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] == '<' || line[i] == '>' || line[ft_strlen(line) - 1] == '<' || line[ft_strlen(line) - 1] == '>')
		return (perror("Wrong syntax"));
	while (line[i] != '\0')
	{
		if (line[i] == '\'')
			quotes++;
		if (line[i] == '\"')
			quotesdouble++;
		i++;
		if (line[i] == '>' && (line[i + 1] == '\0' || line[i + 1] == '>'))
			return (perror("Wrong syntax"));
	}
	if (quotes % 2 == 1 || quotesdouble % 2 == 1)
		return (perror("Wrong syntax"));
}

void	parsing2(char *line, char **envp)
{
	t_command command;
	pid_t pid;

	pid = fork();
	if (pid == -1)
		return (perror("Fork faild"));
	if (pid == 0)
	{
		check_input(line);
	}
	waitpid(pid, NULL, 0);
}

void	parsing(char *line, char **envp)
{
	char **tokens;
	char **argv;
	int i;

	i = 2;
	tokens =  ft_split(line, ' ');
	argv = malloc(4 * sizeof(char *));
	argv[0] = ft_strjoin(argv[0], tokens[1]);
	while (tokens[i][0] != '|')
	{
		argv[1] = ft_strjoin(argv[1], tokens[i]);
		i++;
		if (tokens[i][0] != '|')
			argv[1] = ft_strjoin(argv[1], " ");
	}
	i++;
	while (tokens[i][0] != '>')
	{
		argv[2] = ft_strjoin(argv[2], tokens[i]);
		i++;
		if (tokens[i][0] != '>')
			argv[2] = ft_strjoin(argv[2], " ");
	}
	argv[3] = ft_strjoin(argv[3], tokens[i + 1]);
	pipex(argv, envp);
}

void parse_input(char **envp)
{
	char *line;

	while (1)
	{
		line = readline("minishell: ");
		if (!ft_strncmp(line, "exit", 4))
			break ;
		parsing2(line, envp);
	}
}

int main(int argc, char **hallo, char **envp)
{
	if (!envp || argc < 1)
		return (1);
	parse_input(envp);
}