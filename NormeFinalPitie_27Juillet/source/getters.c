/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:18:20 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/22 13:18:21 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ft_get_delimiter(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != ' '
		&& str[i] != '<' && str[i] != '>'
		&& str[i] != '|')
		i++;
	return (i);
}

char	*get_var_name(const char *str)
{
	char	*name;
	int		i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	name = malloc(i + 1);
	if (!name)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

char	*get_var_value(char *str)
{
	char	*value;

	value = str;
	while (value[0] != '=')
		value++;
	value++;
	value = ft_strdup(value);
	return (value);
}

int	last_redirection_size(char *cmd, char c, int *start)
{
	int	i;

	i = (int)ft_strlen(cmd);
	while (i >= 0) // on demarre de la fin du str
	{
		if (cmd[i] == c && !is_in_quotes(cmd, i)) // break si < ou >
			break ;
		i--;
	}
	if (i == -1) // pas de >
		return (0);
	while (cmd[i] && ft_isspace(cmd[++i]))
		;
	*start = i; //debut dir
	while (cmd[i])
	{
		if (ft_isspace(cmd[i]) && !is_in_quotes(cmd, i)) //break " " 
			break ;
		i++;
	}
	return (i - *start); //taille du doc a malloc
}
