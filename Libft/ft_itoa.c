/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohendrix <ohendrix@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/31 14:10:54 by ohendrix      #+#    #+#                 */
/*   Updated: 2023/10/31 14:12:23 by ohendrix      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	checksize(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		i++;
	while (n != 0)
	{
		i++;
		n = n / 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char			*str;
	int				i;
	unsigned int	number;

	i = checksize(n);
	str = malloc(i + 1);
	number = n;
	if (!str)
		return (NULL);
	if (n < 0)
	{
		str[0] = '-';
		number = -n;
	}
	str[i] = '\0';
	i--;
	while (i >= 0 && str[i] != '-')
	{
		str[i] = (number % 10) + 48;
		number = number / 10;
		i--;
	}
	return (str);
}

// int main(void)
// {
//     int n = -3444;
//     printf("%s", ft_itoa(n));
// }