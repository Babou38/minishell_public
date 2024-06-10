/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 09:36:18 by bchapuis          #+#    #+#             */
/*   Updated: 2024/03/02 14:31:07 by babou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// char	*ft_strchr( const char *s, int search)
// {
// 	if (s[0] == (unsigned char)search)
// 		return ((char *)s);
// 	while (*s)
// 	{
// 		s++;
// 		if (*s == (unsigned char)search)
// 			return ((char *)s);
// 	}
// 	return (0);
// }

int	find_newline(char *stock)
{
	int	i;

	i = 0;
	while (stock[i] != '\n' && stock[i] != '\0')
		i++;
	return (i);
}

char	*ft_get_line(char *stock, char *buff, int nbr)
{
	char	*line;
	int		i;

	if (nbr == -1 || (nbr == 0 && ft_strlen(stock) == 0))
	{
		buff[0] = '\0';
		free(stock);
		return (NULL);
	}
	i = find_newline(stock);
	if (stock[i] == '\n')
	{
		line = ft_substr(stock, 0, i + 1);
		ft_strlcpy(buff, &(stock[i + 1]), BUFFER_SIZE);
	}
	else
	{
		line = ft_strdup(stock);
		buff[0] = '\0';
	}
	free(stock);
	if (line == NULL)
		return (NULL);
	return (line);
}

char	*read_line(int fd, char *stock, char *buff, int *nbr)
{
	char		*tmp;

	while (!(ft_strchr(stock, '\n')))
	{
		*nbr = read(fd, buff, BUFFER_SIZE);
		if (*nbr > 0)
		{
			buff[*nbr] = '\0';
			tmp = ft_strjoin(stock, buff);
			free(stock);
			if (tmp == NULL)
				return (NULL);
			stock = tmp;
		}
		else
			break ;
	}
	return (stock);
}

char	*get_next_line(int fd)
{
	static char	buff[BUFFER_SIZE + 1];
	char		*stock;
	int			nbr;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	nbr = 0;
	stock = ft_strdup(buff);
	if (stock == NULL)
		return (NULL);
	stock = read_line(fd, stock, buff, &nbr);
	if (stock == NULL)
		return (NULL);
	return (ft_get_line(stock, buff, nbr));
}
