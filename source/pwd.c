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

#include "minishell.h"

void ft_pwd(t_shell *shell)
{
    char *pwd;
    
    pwd = getcwd(NULL, 0);
    if (pwd != NULL)
        printf("%s\n", pwd);
    else
        perror("pwd failed");
    free(pwd);
    shell->exitcode = 0;
}