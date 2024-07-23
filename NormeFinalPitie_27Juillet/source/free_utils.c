/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:13:01 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/22 13:13:02 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free(void *ptr)
{
	if (ptr)
		free(ptr);
}

void	safe_exit(t_shell *shell, void *param, t_exitflag flag, int err)
{
	t_env_list	*node;

	if (flag != NONE)
	{
		if (flag == STR)
			ft_free((char *)param);
		else if (flag == NODE)
			free_nodes((t_env_list *)param);
	}
	ft_free(shell->heredoc_name);
	if (shell->split_path)
		ft_free_split(shell->split_path);
	if (shell->env)
		ft_free_split(shell->env);
	if (shell->list)
		ft_free_env(&shell->list);
	if (flag != END)
		ft_free(shell->str);
	free_cmd_list(shell->cmd_list);
	rl_clear_history();
	exit(err);
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
	int	i;

	i = 0;
	while (m_tab[i])
	{
		if (m_tab[i] != NULL)
			free(m_tab[i]);
		i++;
	}
	free(m_tab);
}

char	*strjoin_and_free(char *s1, const char *s2)
{
	char	*new_str;
	char	*temp_str;

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
