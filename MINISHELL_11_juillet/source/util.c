/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:40:05 by bchapuis          #+#    #+#             */
/*   Updated: 2024/06/29 10:24:23 by babou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strndup(const char *s, int n)
{
	// int j;
	char	*str;
	int		i;

	// j = 0;
	str = malloc(sizeof(char) * n + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

void	ft_free_env(t_env_list **head)
{
	t_env_list	*temp;
	while (*head != NULL)
	{
		temp = *head;
		free(temp->value);
		free(temp->var_name);
		*head = (*head)->next;
		free(temp);
	}
}

void	ft_free_split(char **m_tab)
{
		int	 i;

		i = 0;
		// if (!m_tab || !*m_tab) //to check
		// 	return ;
		while (m_tab[i])
		{
				if (m_tab[i] != NULL)
					free(m_tab[i]);
				i++;
		}
		free(m_tab);
}

void free_tokens(t_token **tokens)
{
	int i = 0;

	if (tokens == NULL)
		return ;
	while (tokens[i])
	{
		free(tokens[i]->value);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

char *ft_strcpy(char *dest, const char *src)
{
	char *start = dest;

	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return start;
}

char *strjoin_and_free(char *s1, const char *s2)
{
	char *new_str;
	char *temp_str;

	if (!s1 || !s2)
		return (NULL);
	temp_str = ft_strjoin(s1, " ");
	if (!temp_str)
		return (NULL);
	new_str = ft_strjoin(temp_str, s2);
	free(temp_str);
	free(s1);
	return (new_str);
}

char	*ft_strnstr_i(const char *big, const char *little, size_t len, size_t *i) //N
{
	size_t	j;

	if (*little == '\0')
		return ((char *)big);
	while (big[*i] && *i < len)
	{
		j = 0;
		while (big[*i + j] == little[j] && *i + j < len && little[j])
		{
			if (little[j + 1] == '\0')
				return ((char *)&big[*i]);
			j++;
		}
		(*i)++;
	}
	return (NULL);
}

char	*ft_strdup2(const char *s)
{
	size_t	len;
	char	*str;
	int		i;

	if (s[0] == '\0')
	{
		str = malloc(sizeof(char));
		str[0] = '\0';
		return (str);
	}
	len = ft_strlen(s);
	str = malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

int	is_in_quotes(const char *str, int j)
{
	int	i;
	int	quotes;

	i = 0;
	quotes = 0;
	while (i <= j)
	{
		if ((str[i] == '"' && quotes == 0) || (str[i] == '\'' && quotes < 0))
			quotes++;
		else if ((str[i] == '\'' && quotes == 0) || (str[i] == '"'
				&& quotes > 0))
			quotes--;
		i++;
	}
	if (quotes < 0)
		return (1);
	if (quotes > 0)
		return (2);
	return (0);
}

void	printferr(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
}