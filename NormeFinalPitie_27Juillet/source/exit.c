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

int	check_invalid_char(char *s)
{
	while (*s)
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
	return (code);
}

int	get_exit_code(char *str)
{
	int		code;
	int		err;

	err = 0;
	code = convert_code(str, &err, 1, 0);
	if (code > 255 || code < 0)
		code = correct_exit_code(code);
	if (err > 1)
	{
		printferr(" numeric argument required\n");
		code = 2;
	}
	else
		printf("exit\n");
	return (code);
}

int	ft_is_exit(t_shell *shell, char **v_cmd)
{
	if (v_cmd[1] && v_cmd[2])
	{
		printferr(" too many arguments\n");
		shell->exitcode = 1;
		return (0);
	}
	if (v_cmd[1])
		shell->exitcode = get_exit_code(v_cmd[1]);
	else
	{
		printf("exit\n");
		shell->exitcode = 0;
	}
	return (1);
}
