/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:44:54 by bchapuis          #+#    #+#             */
/*   Updated: 2024/05/02 09:51:59 by marvin           ###   ########.fr       */
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
    TOKEN_CMD,
    TOKEN_ARG,
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

typedef struct s_ast_node 
{
    int type;               // Le type de nœud (par exemple, TOKEN_CMD, TOKEN_PIPE, etc.)
    char *value;            // La valeur associée au nœud (par exemple, le nom de la commande)
    char **args;            // Tableau d'arguments de la commande
    int num_args;           // Nombre d'arguments de la commande
    struct s_ast_node *left; // Sous-arbre gauche
    struct s_ast_node *right; // Sous-arbre droit
} t_ast_node;




typedef struct s_shell
{
    int     exit;
    int     exitcode;
    char    **env;
    char    *str;
    t_token **list_token;
    t_ast_node *ast;
}   t_shell;

//main
char **ft_parsing(t_shell *shell);
void init_envp(t_shell *shell, char **envp);
char *ft_read_input(void);
void    ft_free_split(char **tab);
void handle_command(t_shell *shell);

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

//AST
t_ast_node *create_ast_node(t_token *token);
t_ast_node *build_ast(t_token **tokens);
void execute_ast(t_ast_node *node, t_shell *shell);
void free_ast(t_ast_node *node);

void execute_command(t_ast_node *node, t_shell *shell);
void execute_pipe(t_ast_node *node, t_shell *shell);
#endif