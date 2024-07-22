/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmd_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchapard <lchapard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:50:55 by lchapard          #+#    #+#             */
/*   Updated: 2024/07/08 12:50:57 by lchapard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

#include "../../includes/minishell.h"

static int	size_redirection(char *cmd, int *i, int res)
{
	while (cmd[*i])
	{
		if ((cmd[*i] == '>' || cmd[*i] == '<') && !is_in_quotes(cmd, *i))
			break ;
		*i += 1;
	}
	if (!cmd[*i])
		return (0);
	res = *i;
	*i += 1;
	if ((cmd[*i] == '>' || cmd[*i] == '<') && !is_in_quotes(cmd, *i))
		*i += 1;
	while (cmd[*i] && ft_isspace(cmd[*i]))
		*i += 1;
	while (cmd[*i])
	{
		if (ft_isspace(cmd[*i]) && !is_in_quotes(cmd, *i))
			break ;
		*i += 1;
	}
	res = *i - res;
	return (res);
}

static void	copy_without_redirections(char *cmd, char *result)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd[i])
	{
		if ((cmd[i] == '>' || cmd[i] == '<') && !is_in_quotes(cmd, i))
			size_redirection(cmd, &i, 0);
		if (!cmd[i])
			break ;
		result[j++] = cmd[i++];
	}
	result[j] = '\0';
}

char	*clean_command(char *cmd) //enleve les redirs pour le pas les mettre dans le tableau de tableau
{
	char	*result;
	int		len;
	int		i;

	i = 0;
	len = 0;
	while (cmd[i])
		len += size_redirection(cmd, &i, 0);
	result = malloc(sizeof(char) * (ft_strlen(cmd) - len + 1));
	if (!result)
		return (NULL);
	copy_without_redirections(cmd, result);
	return (result);
}

void	ft_free(void *ptr)
{
	if (ptr)
		free(ptr);
}

t_command	*cmd_last(t_command *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	free_cmd_list(t_command *cmd)
{
	t_command	*tmp;
	int			i;

	cmd = cmd_last(cmd);
	while (cmd)
	{
		tmp = cmd->prev;
		ft_free(cmd->cmd);
		if (cmd->heredoc_fd > 2)
			close(cmd->heredoc_fd);
		ft_free(cmd->input_redirection);
		ft_free(cmd->output_redirection);
		if (cmd->v_cmd)
		{
			i = 0;
			while (cmd->v_cmd[i])
				ft_free(cmd->v_cmd[i++]);
			ft_free(cmd->v_cmd);
		}
		if (cmd->prev)
			ft_free(cmd);
		cmd = tmp;
	}
}

void	*ft_memset2(void *s, int c, size_t n)
{
	char	*ptr;

	ptr = (char *)s;
	while (n--)
	{
		*ptr = (char)c;
		ptr++;
	}
	return (s);
}

size_t	count_pipes(const char *str)
{
	size_t	count;
	int		i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '|' && !is_in_quotes(str, i))
			count++;
		i++;
	}
	return (count);
}