/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_striteri.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 15:02:39 by ohendrix      #+#    #+#                 */
/*   Updated: 2024/05/06 13:28:44 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}

int	ft_stritr(char *str, char *skipped)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while(skipped[j])
		{
			if(str[i] == skipped[j])
				return (i);
			j ++;
		}
		i ++;
	}
	return (i);
}


// void modify_char(unsigned int i, char *c) 
// {
//     *c = *c + i;
// }

// int main(void)
// {
//     char s[] = "abc";
// 	// char s2[] = "abc";
//     ft_striteri(s, modify_char);
// 	// striteri(s2, modify_char);
//     printf("%s\n", s);
// 	// printf("%s", s2);
// }