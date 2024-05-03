/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_megasplit.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 12:16:01 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/03 12:51:35 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_count_left(char *str, char c, int index)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < index)
	{
		if (str[i] == c)
			j ++;
		i ++;
	}
	return (j);
}

int pipe_check(char *str)
{
	int	i;
	int pipes;

	i = 0;
	pipes = 0;
	while (str[i])
	{
		if (str[i] == '|')
		{
			// if ((ft_count_left(str, '\'', i) % 2 == 0) && (ft_count_left(str, '\"', i) % 2 == 0))
			if ((ft_count_left(str, '0', i) % 2 == 0) && (ft_count_left(str, '1', i) % 2 == 0))
				pipes ++;
		}
		i ++;
	}
	return (pipes);
}

// int ft_wordcount(char *str)
// {
	
// }

int main(int argc, char **argv)
{
	int pipes = pipe_check(argv[1]);
	printf("pipes: %d\n", pipes);
	return 0;	
}