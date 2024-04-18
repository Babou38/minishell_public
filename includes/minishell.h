/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchapuis <bchapuis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:44:54 by bchapuis          #+#    #+#             */
/*   Updated: 2024/04/18 14:28:23 by bchapuis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include "libft.h"

typedef enum e_tokentype
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECTION_IN,
    TOKEN_REDIRECTION_OUT,
    TOKEN_INSERTION,
    TOKEN_EXTRACTION,
    TOKEN_SINGLE_QUOTE,
    TOKEN_DOUBLE_QUOTE,
    TOKEN_END
}               t_tokentype;

typedef struct  s_token
{
    t_tokentype type;
    char        *value;
}               t_token;

typedef struct s_shell
{
    char    **env;
    char    **cmd;
    t_token **commande;
    char    *str;
    int     exit;
    int     exitcode;
}   t_shell;

//main
char **ft_parsing(t_shell *shell);
void init_envp(t_shell *shell, char **envp);
char *ft_read_input(void);
void    ft_free_split(char **tab);

//builtins
void    ft_cd(t_shell *shell);
void    ft_pwd(t_shell *shell);
void    ft_echo(t_shell *shell);
void    ft_env(t_shell *shell);
void    ft_unset(t_shell *shell);
int     ft_is_exit(t_shell *shell);
void    ft_export(t_shell *shell);

//builtouts
void    ft_execute(char **cmd, char **envp, t_shell *shell);
char    *get_cmd_path(char *cmd, char **env);
char    *check_path(char **path, char *cmd);
char    *join_path_cmd(char *path, char *cmd);

//error
void    ft_no_path(void);
void    error(char **memory_to_free, int exit_code);

//lexer
t_token *create_token(t_tokentype type, char *value);
t_token **lexer(char *input);
void print_tokens(t_token **tokens);
void free_tokens(t_token **tokens);
char	*ft_strndup(const char *s, int n);
#endif