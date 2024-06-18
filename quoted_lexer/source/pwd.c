/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:23:56 by marvin            #+#    #+#             */
/*   Updated: 2024/04/12 15:23:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_get_cwd(t_shell *shell, int *err)
{
	t_env_list	*head;
	bool		found;
	char		*path;
	
	head = shell->list;
	found = false;
	while (head && head->next)
	{
		if (!ft_strcmp(head->var_name, "PWD") && head->value != NULL)
		{
			found = true;
			path = ft_strdup(head->value);
			return (path);
		}
		head = head->next;
	}
	path = ft_strdup(getcwd(NULL, 0));
	if (!found)
		*err += 1;
	return (path);
}

void ft_pwd(t_shell *shell, t_ast_node *node)
{
    char	*pwd;
	int		err;
    (void)node;
	err = 0;
    pwd = getcwd(NULL, 0);// faux, pas le bon env
	//pwd = ft_get_cwd(shell, &err);
    if (pwd == NULL)
	{
		shell->exitcode = 1;
		perror("pwd failed");
	}
	else
		printf("%s\n", pwd);
	if (pwd)
		free(pwd);
    shell->exitcode = err;
}