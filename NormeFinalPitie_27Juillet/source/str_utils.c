/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:42:09 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/22 13:42:09 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	printferr(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
}

void	print_errors(char *s1, char *s2, char *s3)
{
	ft_putstr_fd(s1, 2);
	ft_putstr_fd(s2, 2);
	ft_putstr_fd(s3, 2);
}

char	*str_without_quotes(char *str)
{
	char			*result;

	//alloc taille sans les quotes
	result = malloc(sizeof(char) * (ft_strlen(str) - count_quotes(str) + 1));
	if (!result)
	{
		perror("minishell: malloc: ");
		free(str);
		return (NULL);
	}
	copy_quoteless(result, str); //recopie sans quote
	free(str);
	return (result);
}

void	copy_quoteless(char *result, char *str)
{
	unsigned int	i;
	unsigned int	j;
	int				qts;

	i = 0;
	j = 0;
	qts = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && qts == 0)
		{
			qts++;
			if (str[i] == '\'')
				qts -= 2;
		}
		else if ((str[i] == '\'' && qts == -1) || (str[i] == '"' && qts == 1))
			qts = 0;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
}

void	copy_without_quotes(char *word, const char *s, unsigned int start,
							unsigned int end)
{
	int				quotes;
	unsigned int	i;

	i = 0;
	quotes = 0;
	while (start <= end)
	{
		if ((s[start] == '\'' || s[start] == '"') && quotes == 0)
		{
			if (s[start] == '\'')
				quotes--;
			else
				quotes++;
		}
		else if (((s[start] == '\'' && quotes == -1)
				|| (s[start] == '"' && quotes == 1)))
			quotes = 0;
		else
			word[i++] = s[start];
		start++;
	}
	word[i - 1] = '\0';
}
