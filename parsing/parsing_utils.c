/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/22 16:23:18 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/06 16:35:52 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void printstruct(t_command *command)
{
	if (command->pipes)
		printf("pipe: %d\n", command->pipes);
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
	if (command->cmd_count)
		printf("cmd_count: %d\n", command->cmd_count);
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

// void	trim_quotes(t_list **list)
// {
// 	t_list *begin;
// 	int		i;

// 	begin = *list;
// 	while (begin != NULL)
// 	{
// 		while (begin->str[i] != '\0')
// 		{
// 			if (begin->str[i] == '\"')
// 				begin->str = trimdquote(begin->str, i);
// 			i++;
// 		}
// 	}
// }

char	*ft_safe_strdup(char *str, t_command *command)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(str) + 1;
	dup = malloc(len);
	if (!dup)
		ft_mallocfail(command, "MALLOC fAILED IN STRDUP");
	ft_strlcpy(dup, str, len);
	return (dup);
}

char	*ft_safe_strjoin(t_command *command, char *s1, char const *s2)
{
	char *str;
	
	str = ft_strjoin(s1, s2);
	if (str == NULL)
	{
		ft_mallocfail(command, "MALLOC FAILED IN STRDUP");
	}
	return (str);
}
