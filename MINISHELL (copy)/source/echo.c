/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:24:19 by marvin            #+#    #+#             */
/*   Updated: 2024/04/12 15:24:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_getenv(t_env_list *list, char *target)
{
	t_env_list *head;
	char	*res;

	res = NULL;
	head = list;
	if (!target[0])
		return (res);
	while (head != NULL)
	{
		// printf("hey i'm looking for %s ?\n", target);
		if (ft_strcmp(target, head->var_name) == 0)
		{
			//printf("found it ! -> %s = %s \n", head->var_name, head->value);
			res = ft_strdup(head->value);
			break;
		}
		else
			head = head->next;
	}
	return (res);
}

bool	contains_global_var(char *str, int *begin, int *end, int i)
{

	while (str[i])
	{
		if (str[i] == '$')
		{
			*begin = i;
			i++;
			if (str[i] == '?')
				i++;
			while (str[i] && ((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= '0' && str[i] <= '9') || str[i] == '_'))
				i++;
			*end = i - 1;
			// printf("begin : [%d], end : [%d]\n", *begin, *end);
			return (true);
		}
		i++;
	}
	return (false);
}

// void	replace_dollar_echo(t_shell *shell ,char *str, int begin, int end)
// {
// 	char	*to_search;
// 	char	*var_value;
// 	int k;

// 	k = 0;
// 	str[begin] = '\0';
// 	printf("%s", str);
// 	str[begin] = '$';
// 	// printf("string after $ : %s\n", str + begin);
// 	// printf("end char : (%c)",str[end]);
// 	to_search = malloc( sizeof(char) * (end - begin + 2));
// 	if (!to_search)
// 		safe_exit(shell, NULL, NONE, 1); // on y va jamais, car on expand au debut du programme
// 	while (begin <= end)
// 	{
// 		to_search[k] = str[begin];
// 		k++;
// 		begin++;
// 	}
// 	to_search[k] = '\0';
// 	// printf("final : %s\n", to_search);
// 	var_value = ft_getenv(shell->list, to_search + 1);
//     if (var_value != NULL)
// 	{
//     	printf("%s", var_value);
// 		free(var_value);
// 	}
// 	if (str[begin])
// 		printf("%s ", str + begin);
// 	free(to_search);
// }

bool	valid_echo_opt(char *opt)
{
	int	i;

	i = 0;
	if (opt[i++] == '-')
	{
		while (opt[i])
		{
			if (opt[i] != 'n')
				return (false);
			i++;
		}
		return (true);
	}
	return (false);
}
void	handle_echo_opt(t_ast_node *node, int i, bool *is_opt, bool *no_newline)
{
	if (*(is_opt) && valid_echo_opt(node->args[i]))
			*(no_newline) = true;
		else
		{
			*(is_opt) = false;
			if (node->args[i + 1] == NULL)
				printf("%s", node->args[i]);
			else
				printf("%s ", node->args[i]);
		}
}

void ft_echo(t_shell *shell, t_ast_node *node) // NEW
{
    int		i;
    bool	no_newline;
	bool	is_opt;

	i = 1;
	is_opt = true;
	no_newline = false;
	if (node->args[1] == NULL)
	{
		printf("\n");
		shell->exitcode = 0;
		return ;
	}
	while (node->args[i] != NULL)
		handle_echo_opt(node, i++, &is_opt, &no_newline);
    if (!no_newline)
		printf("\n");
    shell->exitcode = 0;
}