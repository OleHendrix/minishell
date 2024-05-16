/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   checksyntax.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: olehendrix <olehendrix@student.42.fr>        +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/24 14:18:39 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/14 15:58:31 by ohendrix      ########   odam.nl         */
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
		if ((line[i] == '<' || line[i] == '>' || line[i] == '\0') && i > 1)
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
}
