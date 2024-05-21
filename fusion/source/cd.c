/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:22:59 by marvin            #+#    #+#             */
/*   Updated: 2024/04/12 15:22:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void update_env_var(t_shell *shell, char *var, char *new_value)
{
	t_env_list	*head;
    int i;
    int j;
	bool	found;

    i = -1; // 0 a la base ?? mais ++i sautait le [0]
    j = 0;
	found = false;
	head = shell->list;
	while (head != NULL)
	{
		if (!ft_strcmp(head->var_name, var))
		{
			if (head->value)
				free(head->value);
			head->value = ft_strdup(new_value);
			found = true;
			break ;
		}
		head = head->next;
	}
    if (found)
    {
        while (var[j])
            j++;
        while (shell->env[++i])
        {
            if (ft_strncmp(shell->env[i], var, j) == 0)
            {
                free(shell->env[i]);
				var = ft_strjoin_f(var, "=", false);
                shell->env[i] = ft_strjoin_f(var, new_value, true);
                break ;
            }
        }
    }
}

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

void ft_cd(t_shell *shell)
{
    
    char		*oldpwd;
    char		*newpwd;
	int			err;

	err = 0;
	oldpwd = ft_get_cwd(shell, &err);
    if (shell->ast->args[1] == NULL)
		goto_home_path(shell, &err);
    else
    {
        if (chdir(shell->ast->args[1]) != 0)
			err += 1;
    }
	
    newpwd = getcwd(NULL, 0); // pose un probleme ? 
	if (err)
	{
		if (err < 5)
			perror("cd failed");
		free(oldpwd);
		shell->exitcode = err;
		return ;
	}
	update_env_var(shell, "OLDPWD", oldpwd); // j'ai change le OLDPWD= EN OLDPWD
	update_env_var(shell, "PWD", newpwd);
    free(oldpwd);
    free(newpwd);
    shell->exitcode = 0;
}

