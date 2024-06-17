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
	// int i;

	// i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			*begin = i;
			i++;
			while (str[i] && ((str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122) || (str[i] >= 48 && str[i] <= 57) || str[i] == '_'))
				i++;
			*end = i - 1;
			printf("begin : [%d], end : [%d]\n", *begin, *end);
			return (true) ;
		}
		i++;
	}
	return (false);
}

void	replace_dollar_echo(t_shell *shell ,char *str, int begin, int end)
{
	char	*to_search;
	char	*var_value;
	int k;

	k = 0;
	str[begin] = '\0';
	printf("%s", str);
	str[begin] = '$';
	// printf("string after $ : %s\n", str + begin);
	// printf("end char : (%c)",str[end]);
	to_search = malloc( sizeof(char) * (end - begin + 2));
	
	while (begin <= end)
	{
		to_search[k] = str[begin];
		k++;
		begin++;
	}
	to_search[k] = '\0';
	// printf("final : %s\n", to_search);
	var_value = ft_getenv(shell->list, to_search + 1);
    if (var_value != NULL)
	{
    	printf("%s", var_value);
		free(var_value);
	}
	if (str[begin])
		printf("%s ", str + begin);
	free(to_search);
}

void ft_echo(t_shell *shell, t_ast_node *node)
{
    int i = 1;
	int	begin = 0;
	int	end = 0;
    bool add_newline = true;

    if (node->args[i] != NULL && ft_strcmp(node->args[i], "-n") == 0)
    {
        add_newline = false;
        i++;
    }
    while (node->args[i] != NULL)
    {
		// printf("\ns :%s \n", node->args[i]);
        if (ft_strcmp(node->args[i], "$?") == 0)
            printf("%d", shell->exitcode);
		if (contains_global_var(node->args[i], &begin, &end, 0))
			replace_dollar_echo(shell, node->args[i], begin, end);
        else
            printf("%s ", node->args[i]);
        i++;
    }
    if (add_newline)
        printf("\n");
    shell->exitcode = 0;
}