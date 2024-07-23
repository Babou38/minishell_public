/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 17:04:39 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/18 17:04:40 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	goto_home_path(t_shell *shell, int *err)
{
	t_env_list	*head;
	bool		found;

	head = shell->list;
	found = false;
	while (head != NULL)
	{
		if (!ft_strcmp(head->var_name, "HOME"))
		{
			found = true;
			if (chdir(head->value) != 0)
				*err += 1;
			return ;
		}
		head = head->next;
	}
	printf("cd: HOME not set\n");
	if (!found)
		*err += 5;
}
