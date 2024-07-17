/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 09:37:26 by marvin            #+#    #+#             */
/*   Updated: 2024/04/18 09:37:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token *create_token(t_tokentype type, char *value)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (token->value == NULL)
	{
		free(token);
		return (NULL);
	}
	return (token);
}

int		find_final_quote(char *s, char c) //N
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			return (i);
		}
		i++;
	}
	return (i);
}

char	*remove_first_quotes(char *s, int i, int j) //N
{
	char	*res;
	bool	is_quoted;
	int		final_quote;

	is_quoted = false;
	final_quote = -1;
	res = malloc(sizeof(char) * ft_strlen(s) + 1);
	while (s[i])
	{
		if (final_quote != -1 && i == final_quote)
			i++;
		if (s[i] && (s[i] == '"' || s[i] == '\'') && !is_quoted)
		{
			is_quoted = true;
			final_quote = find_final_quote(s, s[i]);
			i++;
		}
		else
			res[j++] = s[i++];
	}
	res[j] = '\0';
	// printf("final : (%s)\n\n", res);
	return res;

}

char	*isolate_command(char *input, int *k, int i) //N
{
	char	*res;
	// char	*parsed_res;

	while (input[*k])
	{
		if ((input[*k] == '|' || input[*k] == '<' || input[*k] == '>') && !is_in_quotes(input, *k))
			break ;
		if (input[*k] == '\0' || (input[*k] == ' ' && !is_in_quotes(input, *k)))
			break ;
		(*k)++;
	}
	res = ft_strndup(input + i, *k - i);
	// printf("ressss : %s\n\n", res);
	// parsed_res = remove_first_quotes(res, 0, 0); //NEW
	// free(res);
	// return (parsed_res);
	

	return (res);
}

t_token **lexer(char *input, bool *is_valid) //N
{
	t_token **tokens;
	char *word;
	int i, j, k;
	int is_command;

	tokens = malloc(sizeof(t_token *) * (ft_strlen(input) + 1));  /// to check
	if (tokens == NULL)
	{	
		*is_valid = false;
		return tokens;
	}
	i = 0;
	j = 0;
	is_command = 1;
	while (input[i])
	{
		if (input[i] != ' ')
		{
			if (input[i] == '|')
			{
				tokens[j++] = create_token(TOKEN_PIPE, "|");
				i++;
				is_command = 1;
			}
			else if (input[i] == '<' && input[i + 1] == '<')
			{
				tokens[j++] = create_token(TOKEN_INSERTION, "<<");
				i = i + 2;
				is_command = 0; //N
			}
			else if (input[i] == '>' && input[i + 1] == '>')
			{
				tokens[j++] = create_token(TOKEN_EXTRACTION, ">>");
				i = i + 2;
			}
			else if (input[i] == '<')
			{
				tokens[j++] = create_token(TOKEN_REDIRECTION_IN, "<");
				i++;
				is_command = 1;
			}
			else if (input[i] == '>')
			{
				tokens[j++] = create_token(TOKEN_REDIRECTION_OUT, ">");
				i++;
				is_command = 1;
			}
			// else if (input[i] == '\'' || input[i] == '\"') ////////////// ------> ancien, obsolete
			// {
			// 	char quote = input[i];
			// 	if (input[i + 1] != quote)
			// 	{
			// 		k = ++i;
			// 		while (input[i] && input[i] != quote)
			// 			i++;
			// 		word = ft_strndup(input + k - 1, i - k + 2); // to check + changed
			// 		if (quote == '\'')
			// 			tokens[j++] = create_token(TOKEN_SINGLE_QUOTE, word);
			// 		else
			// 			tokens[j++] = create_token(TOKEN_DOUBLE_QUOTE, word);
			// 		free(word);
			// 		i++;
			// 	}
			// 	else
			// 		i += 2;
			// }
			else
			{
				k = i;
				word = isolate_command(input, &k, i);  //NEW  //////////////////////////////////
				if (!word)
				{
					*is_valid = false;
					tokens[j] = NULL;
					return (tokens);
				}
				if (is_command)
				{
					tokens[j++] = create_token(TOKEN_CMD, word);
					is_command = 0;
				}
				else
				{
					if (j >= 2 && (tokens[j - 2]->type == TOKEN_REDIRECTION_IN || tokens[j - 2]->type == TOKEN_INSERTION))
                            tokens[j++] = create_token(TOKEN_CMD, word);
                    else
                        tokens[j++] = create_token(TOKEN_ARG, word);
				}
				free(word);
				i = k;
			}
			if (tokens[j - 1] == NULL)
			{
				*is_valid = false;
				return (tokens);
			}
		}
		else
			i++;
	}
	tokens[j] = NULL;
	*is_valid = true;
	return (tokens);
}

void print_tokens(t_token **tokens)
{
	int i = 0;

	while (tokens[i])
	{
		printf("Token %d: ", i);
		printf("Type: %d ", tokens[i]->type);
		printf("Value: %s\n", tokens[i]->value);
		i++;
	}
}
