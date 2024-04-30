#include "../minishell.h"

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
		if (!ft_strncmp(tokens[i], ">", 2))
		{
			ft_set_files(command, tokens[i + 1], 1);
			i++;
		}
		i++;
	}
}

void combine_empty_quote(t_command *command)
{
	t_list *current;

	current = command->commands;

	if (current == NULL)
		return ;
	while (current->next != NULL)
	{
		if (!ft_strncmp(current->str, "\"", 2))
		{
			current->str = ft_strjoin(current->str, current->next->str);
			if (!current->str)
				ft_mallocfail(command, "ERROR IN combine_empty_qoute");
			current->next = current->next->next;
		}
		else
			current = current->next;
	}
}

int	count_commands(t_list **list)
{
	t_list *current;
	int		i;

	i = 0;
	current = *list;
	while (current)
	{
		current = current->next;
		i ++;
	}
	return (i);
}

void	print_list(char **str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		printf("%s\n", str[i]);
		i ++;
	}
	// return (i);
}

void fill_struct(char *line, char **envp, char *mode)
{
	t_command	command;

	(void)envp;
	init_struct(&command);
	ft_supersplit(line, ' ', &command);
	// print_list(command.tokens);
	if (!command.tokens)
		return (ft_putstr_fd("ERROR IN SPLIT", 2));
	init_files(&command, command.tokens);
	init_commands(&command, command.tokens);
	command.cmd_count = count_commands(&command.commands);
	// trim_quotes(&command.commands);
	if (mode && !ft_strncmp(mode, "test", 5))
		printstruct(&command);
	else
	{
		command.pid = fork();
		if (!command.pid)
			pipex(envp, &command);
		else
			waitpid(command.pid, NULL, 0);
	}
	ft_free_struct(&command);
	free(line);
}
