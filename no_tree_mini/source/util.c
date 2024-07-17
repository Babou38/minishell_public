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

bool	char_sym(const char s, const char *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (charset[i++] == s)
			return (1);
	}
	return (0);
}

int	ft_isspace(int c)
{
	return ((c >= 9 && c <= 13) || c == 32);
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

bool	is_there_chr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c && !is_in_quotes(str, i))
			return (true);
		i++;
	}
	return (false);
}

void	printferr(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
}

char	*next_redirection_name(t_command *cmd, int i) // prends la derniere redir
{
	int		len;
	char	*redirection;

	while (cmd->cmd[i] == '<' || cmd->cmd[i] == '>' || ft_isspace(cmd->cmd[i]))
		i++;
	len = 0;
	while (cmd->cmd[i + len] && (!ft_isspace(cmd->cmd[i + len])
			|| is_in_quotes(cmd->cmd, i + len)))
		len++;
	redirection = malloc(sizeof(char) * (len + 1));
	if (!redirection)
		return (NULL);
	redirection[len] = '\0';
	while (--len >= 0)
		redirection[len] = cmd->cmd[i + len];
	if (is_there_chr(redirection, '\'') || is_there_chr(redirection, '"'))
		redirection = str_without_quotes(redirection);
	return (redirection);
}

bool	check_last_redirection(char *str, char c)
{
	unsigned int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c && !is_in_quotes(str, i))
			return (false);
		i++;
	}
	return (true);
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
	while (cmd[i] && ft_isspace(cmd[++i])) // skip les espaces entre > et le path
		;
	*start = i; //debut dir
	while (cmd[i])
	{
		if (ft_isspace(cmd[i]) && !is_in_quotes(cmd, i)) // break sur espace
			break ;
		i++;
	}
	return (i - *start); //taille du doc a malloc
}

// int	count_quotes(const char *str)
// {
// 	int	i;
// 	int	qts;
// 	int	count;

// 	i = 0;
// 	qts = 0;
// 	count = 0;
// 	while (str[i])
// 	{
// 		if ((str[i] == '\'' || str[i] == '"') && qts == 0)
// 		{
// 			if (str[i] == '\'')
// 				qts--;
// 			else
// 				qts++;
// 			count++;
// 		}
// 		else if ((str[i] == '\'' && qts == -1) || (str[i] == '"' && qts == 1))
// 		{
// 			qts = 0;
// 			count++;
// 		}
// 		i++;
// 	}
// 	return (count);
// }

void	print_errors(char *s1, char *s2, char *s3)
{
	ft_putstr_fd(s1, 2);
	ft_putstr_fd(s2, 2);
	ft_putstr_fd(s3, 2);
}