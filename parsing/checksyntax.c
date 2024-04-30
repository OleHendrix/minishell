/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checksyntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olehendrix <olehendrix@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:18:39 by ohendrix          #+#    #+#             */
/*   Updated: 2024/04/25 14:58:34 by olehendrix       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool check_chars(char *line, char c)
{
	int	i;

	i = 1;
	while (line[i])
	{
		while (line[i] == ' ')
			i ++;
		if (line[i] == '<' || line[i] == '>' || line[i] == '\0')
			return (false);
		if (c == '|')
		{
			if (line[i] == ' ' && line[i + 1] && line[i + 1] == '|')
				return (false);
		}
		return (true);
	}
	return (false);
}

bool check_quotes(char *line)
{
	int	i;
	bool d_open;
	bool s_open;

	i = 0;
	s_open = false;
	d_open = false;
	if (line[i] == '|')
		return (false);
	while (line[i])
	{
		if ((line[i] == '>' || line[i] == '<' || line[i] == '|') && !d_open && !s_open)
		{
			if (check_chars(line + i, line[i]) == false)
				return (false);
		}
		if (line[i] == '\"' && !s_open)
			d_open = !d_open;
		if (line[i] == '\'' && !d_open)
			s_open =! s_open;
		i ++;
	}
	if (d_open || s_open)
		return (false);
	return (true);
}

bool	checksyntax(char *line)
{
	bool valid;

	valid = check_quotes(line);
	return (valid);
	// if (valid == true)
	// 	printf("OK\n");
	// if (valid == false)
	// 	printf("NIET OK\n");
}

//  < infile "a" "a" kdjkfj """ > outile