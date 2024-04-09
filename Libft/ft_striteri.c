/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_striteri.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 15:02:39 by ohendrix      #+#    #+#                 */
/*   Updated: 2023/11/01 16:49:01 by ohendrix      ########   odam.nl         */
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