/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchapuis <bchapuis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 14:57:35 by bchapuis          #+#    #+#             */
/*   Updated: 2024/05/21 15:05:49 by bchapuis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	convert_code(char *str, int *err, int sign, long int total)
{
	size_t	i;

	i = 0;
	while (str[i] && (str[i] == 32 || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == 43 || str[i] == 45)
	{
		if (str[i++] == 45)
			sign *= -1;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{ 
		total = total * 10 + (str[i++] - 48);
		if ((total > 2147483648 && sign < 0) || (total > 2147483647
				&& sign > 0))
		{
			*err = 1; // manque le valid si char apres
			return (total * sign);
		}
	}
	return (total * sign);
}

bool	str_is_num(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
			if ((str[i] < 48 || str[i] > 57))
				return (false);
			i++;
	}
	return (true);
}

int	get_exit_code(char *str)
{
	int		code;
	int		err;

	err = 0;
	code = convert_code(str, &err, 1 , 0);
	if (err || code > 255 || code < 0)
		code = 1;
	///Selon le man, si le nombre qui suit exit n'est pas entre 0 et 255, alors
	//le exit status est pas defini, donc ici je le fixe a 1 par defaut 

	if (!str_is_num(str)) // si il contient autre chose que des chiffres
	{
		printf("exit: %s : numeric argument required\n", str);/// print dans channel erreur 
		code = 2;
	}
	else
		printf("exit\n");
	return (code);
}

int ft_is_exit(t_shell *shell, t_ast_node *node)
{
	if (node->args[1] != NULL)
    	shell->exitcode = get_exit_code(node->args[1]); 
	else
	{
		printf("exit\n");
		shell->exitcode = 0;
	}
    return (1);
}


