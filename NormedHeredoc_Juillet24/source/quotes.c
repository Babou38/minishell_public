/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 13:56:53 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/16 13:56:55 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_quotes(const char *str)
{
	int	i;
	int	qts;
	int	count;

	i = 0;
	qts = 0;
	count = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && qts == 0)
		{
			if (str[i] == '\'')
				qts--;
			else
				qts++;
			count++;
		}
		else if ((str[i] == '\'' && qts == -1) || (str[i] == '"' && qts == 1))
		{
			qts = 0;
			count++;
		}
		i++;
	}
	return (count);
}

bool	are_quotes_closed(const char *str)
{
	int	i;
	int	quotes;

	i = 0;
	quotes = 0;
	while (str[i])
	{
		if ((str[i] == '"' && quotes == 0) || (str[i] == '\'' && quotes < 0))
			quotes++;
		else if ((str[i] == '\'' && quotes == 0)
			|| (str[i] == '"' && quotes > 0))
			quotes--;
		i++;
	}
	if (quotes == 0)
		return (false);
	return (true);
}
