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

void	safe_exit(t_shell *shell, void *param, t_exitflag flag, int err)  //N 
{
	t_env_list	*node;
	char		*str;

	if (flag != NONE)
	{
		if (flag == STR)
		{
			str = (char *)param;
			free(str);
		}
		else if (flag == NODE)
		{
			node = (t_env_list *)param;
			if ((node->var_name))
				free(node->var_name);
			free(node);
		}
	}
	free(shell->heredoc_name);
	ft_free_split(shell->env);
	ft_free_env(&shell->list);
	if (flag != END)
	{
		free_ast(shell->ast);
		free_tokens(shell->list_token);
		if (shell->str)
			free(shell->str);
	}
	rl_clear_history();
	exit(err);
}

int	check_invalid_char(char *s)
{
	while(*s)
	{
		if (*s < '0' || *s > '9')
			return (2);
		s++;
	}
	return (0);
}

int	convert_code(char *str, int *err, int sign, long int total) //N TODO
{
	size_t	i;

	i = 0;
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
			return (total * sign);
	}
	*err += check_invalid_char(str + i);
	return (total * sign);
}

int	correct_exit_code(int input)
{
	int	code;

	code = input;
	while (code > 255)
	{
		code = code - 256;
	}
	while (code < 0)
	{
		code = code + 256;
	}
	return code;
}

int	get_exit_code(char *str)
{
	int		code;
	int		err;

	err = 0;
	code = convert_code(str, &err, 1 , 0);
	if (code > 255 || code < 0)
		code = correct_exit_code(code);
	///Selon le man, si le nombre qui suit exit n'est pas entre 0 et 255, alors
	//le exit status est pas defini, donc ici je le fixe a 1 par defaut 
	if (err > 1) // si il contient autre chose que des chiffres
	{
		printferr(" numeric argument required\n");
		code = 2;
	}
	else
		printf("exit\n");
	return (code);
}

int ft_is_exit(t_shell *shell, char **v_cmd)
{
	if (v_cmd[2] != NULL)
	{
		printferr(" too many arguments\n");
		shell->exitcode = 1;
		return (0);
	}
	if (v_cmd[1] != NULL)
		shell->exitcode = get_exit_code(v_cmd[1]); 
	else
	{
		printf("exit\n");
		shell->exitcode = 0;
	}
	return (1);
}

