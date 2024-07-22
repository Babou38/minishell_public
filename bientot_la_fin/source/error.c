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

void	error(t_shell *shell, int exit_code)
{
	perror("Error");
	safe_exit(shell, NULL, NONE, exit_code);
}

void	ft_no_path(t_shell *shell)
{
	printf("PATH not found\n");
	safe_exit(shell, NULL, NONE, 127);
}

