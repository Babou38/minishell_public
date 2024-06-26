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

void    error(char **memory_to_free, int exit_code)
{
        if (memory_to_free != NULL)
                ft_free_split(memory_to_free);
        perror("Error");
        exit(exit_code);
}

//si PATH est unset faut utiliser un PATH absolue au lieu d'utiliser cette fonction
void    ft_no_path(void)
{
        write(2, "PATH not found\n", 15);
        exit(127);
}

