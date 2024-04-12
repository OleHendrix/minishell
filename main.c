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


void parse_input(char **envp)
{
	char *line;

	while (1)
	{
		line = readline("minishell: ");
		if (!ft_strncmp(line, "exit", 5))
			break ;
		ft_parsing(line, envp);
	}
}

int main(int argc, char **hallo, char **envp)
{
	if (!envp || argc < 1)
		return (1);
	parse_input(envp);
}