/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: izouiten <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 15:14:33 by izouiten          #+#    #+#             */
/*   Updated: 2019/04/04 15:14:36 by izouiten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hds/wolf3D.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*ptr;
	char	*str;

	str = (char *)s1;
	i = strlen(str);
	ptr = (char *)malloc(sizeof(char) * (i + 1));
	if (ptr == NULL)
		return (0);
	i = 0;
	while (str[i])
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
