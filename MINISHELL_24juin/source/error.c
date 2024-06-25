/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 11:00:47 by marvin            #+#    #+#             */
/*   Updated: 2024/04/13 11:00:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error(t_shell *shell, char **memory_to_free, int exit_code)
{
	if (memory_to_free != NULL)
			ft_free_split(memory_to_free);
	perror("Error");
	safe_exit(shell, NULL, NONE, exit_code);
}

void	ft_no_path(t_shell *shell)
{
	write(2, "PATH not found\n", 15);
	safe_exit(shell, NULL, NONE, 127);
}

