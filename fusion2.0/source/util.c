/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:40:05 by bchapuis          #+#    #+#             */
/*   Updated: 2024/06/10 11:15:41 by babou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ft_strlen_c(const char *str, char c, bool *found) //TO ADD
{
	size_t	i;

	i = 0;
	while (str[i] != c && str[i])
		i++;
	if (str[i] == c)
		*(found) = true;
	return (i);
}

char	*ft_strdup_c(const char *s, int *j, char c, bool *found) //TO ADD
{
	size_t	len;
	char	*str;
	int		i;

	len = ft_strlen_c(s, c, found);
	str = malloc(sizeof(char) * len + 1); // MALLOC
	if (!str)
		return (NULL);
	i = 0;
	while (s[i] != c && s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	*j = ++i;
	return (str);
}

int	ft_strcmp_c( const char *first, const char *second, char c)
{
	size_t	i;

	i = 0;
	while (first[i] && second[i] && (first[i] == second[i]) 
		&& second[i] != c && first[i] != c)
		i++;
	if ((second[i] == '=' && !first[i]) || (first[i] == '=' && !second[i]))
		return (0);
	return ((unsigned char)first[i] - (unsigned char)second[i]);
}

int	ft_strcmp( const char *first, const char *second)
{
	size_t	i;

	i = 0;
	while (first[i] && second[i] && (first[i] == second[i]))
		i++;
	return ((unsigned char)first[i] - (unsigned char)second[i]);
}

char	*ft_strndup(const char *s, int n)
{
	char	*str;
	int		i;

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

void    ft_free_split(char **tab)
{
        int     i;

        i = 0;
        while (tab[i])
        {
                free(tab[i]);
                i++;
        }
        free(tab);
}

void free_tokens(t_token **tokens)
{
    int i = 0;

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

static char	*alloc_join(char const *s1, char const *s2)
{
	char	*str;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	return (str);
}

char	*ft_strjoin_f(char *s1, char *s2, bool to_free)
{
	size_t		i;
	char		*str;

	i = 0;
	str = alloc_join(s1, s2);
	if (str == NULL)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (*s2)
	{
		str[i] = *s2;
		s2++;
		i++;
	}
	if (to_free)
		free(s1);
	str[i] = '\0';
	return (str);
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