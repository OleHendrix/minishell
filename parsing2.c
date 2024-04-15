#include "minishell.h"

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

void create_list(t_list **list, char **tokens)
{
	int i;

	i = 0;
	while (tokens[i] != NULL)
	{
		ft_addnode(list, tokens[i]);
		i++;
	}
}

// void	combine_node(t_list **list)
// {
// 	t_list *begin;
// 	bool	open;

// 	begin = *list;
// 	open = false;
// 	while (begin->next != NULL)
// 	{
// 		if (begin->str[0] == '\"')
// 			open = true;
// 		while (open)
// 		{
// 			begin->str = ft_strjoin(begin->str, begin->next->str);
// 			begin = begin->next;
// 			if (begin->str[ft_strlen(begin->str) - 1] == '\"')
// 				open = false;
// 		}
// 		begin = begin->next;
// 	}
// }

void	combine_node(t_list **list)
{
	t_list *begin;

	begin = *list;
	while (begin->next != NULL)
	{
		if (begin->str[0] != '\"')
		{	
			begin = begin->next;
		}
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
}

void fill_struct2(char *line, char **envp)
{
	t_command	command;
	t_list		*list;
	int			i;
	char		**tokens;

	init_command(&command);
	i = 0;
	list = NULL;
	tokens = ft_split(line, ' ');
	create_list(&list, tokens);
	combine_node(&list);
	printstack(&list);

}