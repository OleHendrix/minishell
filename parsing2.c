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
	if (command->here_doc)
		printf("here doc: true\n");
	if (command->delimiter)
		printf("delimiter: %s\n", command->delimiter);
	if (command->commands)
	{
		printf("commands:\n");
		printstack(&command->commands);
	}
}

void init_command(t_command *command)
{
	command->infile = NULL;
	command->commands = NULL;
	command->outfile = NULL;
	command->here_doc = false;
	command->delimiter = NULL;
	command->pipe = false;
}

void	printstack(t_list **a)
{
	t_list	*begin;

	begin = *a;
	while (begin != NULL)
	{
		printf("%s\n", begin->str);
		begin = begin->next;
	}
}

void	ft_addnode(t_list **list, char *token)
{
	t_list	*newnode;
	t_list	*lastnode;

	newnode = malloc(sizeof(t_list));
	newnode->str = ft_strdup(token);
	newnode->next = NULL;
	if (*list == NULL)
	{
		*list = newnode;
		return ;
	}
	lastnode = *list;
	while (lastnode->next != NULL)
		lastnode = lastnode->next;
	lastnode->next = newnode;
}
void	trim_quotes(t_list **list)
{
	t_list	*begin;

	begin = *list;
	if (*list == NULL)
		return ;
	while ((*list) != NULL)
	{
		if ((*list)->next != NULL && (*list)->next->str[0] == '\"')
		{
			(*list)->next->str = ft_strtrim((*list)->next->str, "\"");
			(*list)->str = ft_strjoin((*list)->str, " ");
			(*list)->str = ft_strjoin((*list)->str, (*list)->next->str);
			(*list)->next = (*list)->next->next;
		}
		(*list) = (*list)->next;
	}
	*list = begin;
}

void	combine_node(t_list **list)
{
	t_list *begin;

	begin = *list;
	if (begin == NULL)
		return ;
	while (begin->next != NULL)
	{
		// if (!ft_strncmp(begin->str, "\" ", 2))
		// {
		// 	begin->str = ft_strjoin(begin->str, begin->next->str);
		// 	begin->next = begin->next->next;
		// 	combine_node(&begin);
		// }
		if (begin->str[0] != '\"')
			begin = begin->next;
		else if (begin->str[0] == '\"' && begin->str[ft_strlen(begin->str) -1] != '\"')
		{
			begin->str = ft_strjoin(begin->str, " ");
			begin->str = ft_strjoin(begin->str, begin->next->str);
			begin->next = begin->next->next;
			combine_node(&begin);
		}
		else
			begin = begin->next;
	}
	trim_quotes(list);
}

void init_files(t_command *command, char **tokens)
{
	int	i;
	bool inquotes;

	i = 0;
	inquotes = false;
	while (tokens[i] != NULL)
	{
		if (tokens[i][0] == '\"' && !inquotes)
			inquotes = true;
		if (ft_strlen(tokens[i]) > 1 && tokens[i][ft_strlen(tokens[i]) - 1] == '\"')
			inquotes = false;
		if (!ft_strncmp(tokens[i], "<", 2) && !inquotes)
		{
			command->infile = ft_strdup(tokens[i + 1]);
			i++;
		}
		if (!ft_strncmp(tokens[i], ">", 2) && !inquotes)
		{
			command->outfile = ft_strdup(tokens[i + 1]);
			i++;
		}
		i++;
	}
}


int ft_flags(char **tokens, int j)
{
	if (tokens[j + 1] == NULL && tokens[j][0] != '\"')
		return (1);
	if (tokens[j + 1] == NULL && tokens[j][0] == '\"')
	{
		tokens[j] = ft_strtrim(tokens[j], "\"");
		return (1);
	}
	if (!ft_strncmp(tokens[j + 1], "|", 2) && tokens[j][0] != '\"')
		return (1);
	if (!ft_strncmp(tokens[j + 1], ">", 2) && tokens[j][0] != '\"')
		return (1);
	return (0);
}

int	ft_checkflags(t_list **commands, char **tokens, int j)
{
	t_list	*lastnode;

	if (*commands == NULL)
		return (0);
	lastnode = *commands;
	while (lastnode->next != NULL)
			lastnode = lastnode->next;
	if (ft_flags(tokens, j))
	{
		if (j > 0 && ft_strncmp(tokens[j - 1], "|", 2))
		{
			lastnode->str = ft_strjoin(lastnode->str, " ");
			lastnode->str = ft_strjoin(lastnode->str, tokens[j]);
			return (1);
		}
	}
	return (0);
}
void	addcommand(t_list **commands, char **tokens, int j)
{
	t_list	*newnode;
	t_list	*lastnode;

	if (ft_checkflags(commands, tokens, j))
		return;
	newnode = malloc(sizeof(t_list));
	newnode->str = ft_strdup(tokens[j]);
	newnode->next = NULL;
	if (*commands == NULL)
	{
		*commands = newnode;
		return ;
	}
	lastnode = *commands;
	while (lastnode->next != NULL)
		lastnode = lastnode->next;
	lastnode->next = newnode;
}

void	init_commands(t_command *command, char **tokens)
{
	int i;
	bool inquotes;

	i = 0;
	inquotes = false;
	while (tokens[i] != NULL)
	{
		if (tokens[i][0] == '\"')
			inquotes = true;
		if (ft_strlen(tokens[i]) > 1 && tokens[i][ft_strlen(tokens[i]) - 1] == '\"')
			inquotes = false;
		if ((!ft_strncmp(tokens[i], "<", 2) || !ft_strncmp(tokens[i], ">", 2)) && !inquotes)
			i++;
		else if (!ft_strncmp(tokens[i], "|", 2) && !inquotes)
			command->pipe = true;
		else if (!ft_strncmp(tokens[i], "<<", 3) && !inquotes)
		{
			command->here_doc = true;
			command->delimiter = ft_strdup(tokens[i + 1]);
			i++;
		}
		else
			addcommand(&command->commands, tokens, i);
		i++;
	}
}

void combine_empty_quote(t_list **list)
{
	t_list *begin;

	begin = *list;

	if (begin == NULL)
		return ;
	while (begin->next != NULL)
	{
		if (!ft_strncmp(begin->str, "\"", 2))
		{
			begin->str = ft_strjoin(begin->str, begin->next->str);
			begin->next = begin->next->next;
		}
		else
			begin = begin->next;
	}
}

void fill_struct2(char *line, char **envp)
{
	t_command	command;
	int			i;
	char		**tokens;

	init_command(&command);
	i = 0;
	tokens = ft_split(line, ' ');

	init_files(&command, tokens);
	init_commands(&command, tokens);
	combine_empty_quote(&command.commands);
	combine_node(&command.commands);
	// pipex(&command, envp);
	printstruct(&command);
}