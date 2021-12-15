/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 19:24:00 by cclaude           #+#    #+#             */
/*   Updated: 2021/09/11 19:24:25 by cclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

int hex_value (char c)
{
	if (c >= 'a')
		return (c - 'W');
	return (c - '0');
}

void fill_mac (unsigned char *addr, char *mac)
{
	for (int i = 0, j = 0 ; i < 6 ; i++, j += 3)
		addr[i] = 16 * hex_value(mac[j]) + hex_value(mac[j + 1]);
}

void memcopy (unsigned char *src, unsigned char *dst, size_t n)
{
	if (!src || !dst)
		return ;
	while (n--)
		*src++ = *dst++;
}

int	cmp(const char *s1, const char *s2)
{
	size_t	i;

	if (s1 == NULL || s2 == NULL)
		return (0);
	if (s1 == NULL || s2 == NULL)
		return (1);
	i = 0;
	while (s1[i] == s2[i])
	{
		if (s1[i] == '\0' && s2[i] == '\0')
			return (0);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}