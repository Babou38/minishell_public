/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchapuis <bchapuis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:40:05 by bchapuis          #+#    #+#             */
/*   Updated: 2024/05/21 14:55:32 by bchapuis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../includes/minishell.h"

char	*ft_strndup(const char *s, int n)
{
	int j;
	char	*str;
	int		i;

    j = 0;
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