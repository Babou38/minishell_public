/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:44:54 by bchapuis          #+#    #+#             */
/*   Updated: 2024/06/10 11:16:35 by babou            ###   ########.fr       */
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
#include <sys/stat.h> //N
#include <stdbool.h>
#include <curses.h>
#include <errno.h>
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
}			   t_tokentype;

typedef enum s_exitflag
{
	NODE,
	STR,
	NONE,
	END,
}			t_exitflag;

typedef struct  s_token
{
	t_tokentype type;
	char		*value;
}			   t_token;

typedef struct s_ast_node 
{
	int type;				   // Le type de nœud (par exemple, TOKEN_CMD, TOKEN_PIPE, etc.)
	char *value;				// La valeur associée au nœud (par exemple, le nom de la commande)
	char **args;				// Tableau d'arguments de la commande
	// int num_args;			   // Nombre d'arguments de la commande
	struct s_ast_node *left;	// Sous-arbre gauche
	struct s_ast_node *right;   // Sous-arbre droit
} t_ast_node;

struct s_env_list
{
	struct s_env_list	*next;
	char				*var_name;
	char				*value;
};
typedef struct s_env_list	t_env_list;

typedef struct s_shell
{
	int	 exit;
	int	 exitcode;
	char	**env;
	char	*str;
	bool	heredoc_quoted;
	int	 heredoc_counter;
	char	*heredoc_name;
	t_env_list	*list;
	t_token **list_token;
	t_ast_node *ast;
}   t_shell;

//main
void		signal_handler_c(int signum);
void		set_signals(void);

//prompt
char		*ft_strjoin_f(char *s1, char *s2, bool to_free);
char		*path_string(t_shell *shell);
char		*handle_incomplete_pipe(char *input);
bool		are_quotes_closed(const char *input);
char		*handle_quotes(t_shell *shell, char *input);
char		*ft_read_input(t_shell *shell);
bool		contains_global_var(char *str, int *begin, int *end, int i); //N

//heredoc
char		*get_env_value(t_shell *shell, char *var_name);
char		*expand_variables(t_shell *shell, const char *input);
char		*read_heredoc(t_shell *shell, char *delimiter);
void		handle_heredoc(t_shell *shell, char *delimiter);
char		*extract_delimiter(t_shell *shell, char *heredoc_pos);

//init_env
int			del_envlist(t_shell **shell, char *name);
void		add_envlist(t_shell **shell, t_env_list **new_node, char *var, bool lone); //N
void		init_envp(t_shell *shell, char **envp);

//lexer
t_token	 *create_token(t_tokentype type, char *value);
t_token	 **lexer(char *input, bool *is_valid);
void		print_tokens(t_token **tokens);

//built_ast
t_ast_node  *create_ast_node(t_token *token, bool debug);
bool		add_argument_to_command(t_ast_node *command_node, char *argument, bool debug); //N BOOL POUR MALLOC
//t_ast_node  *create_command_node(t_token **tokens, int *i, bool debug);
t_ast_node  *create_command_node(t_token **tokens, int *i, t_ast_node *first, bool debug); //N
t_ast_node  *handle_token_type(t_token **tokens, int *i, t_ast_node *node, bool debug);
t_ast_node  *build_ast(t_token **tokens, bool debug);

//execute_ast
void		execute_ast(t_ast_node *node, t_shell *shell);
void		free_ast(t_ast_node *node);
void		printAST(t_ast_node* node);

//execute_command
// void		handle_command(t_shell *shell);
void		handle_command(t_ast_node *node, t_shell *shell); //N
void		execute_command(t_ast_node *node, t_shell *shell);

//execute_pipe
void		execute_pipe(t_ast_node *node, t_shell *shell);

//execute_redirection
void		execute_redirection_out(t_ast_node *node, t_shell *shell);
void		execute_redirection_in(t_ast_node *node, t_shell *shell);
void		execute_extraction(t_ast_node *node, t_shell *shell);
void		execute_heredoc(t_ast_node *node, t_shell *shell);

//pwd
char		*ft_get_cwd(t_shell *shell, int *err); //N
void		ft_pwd(t_shell *shell, t_ast_node *node);

//exit
void		safe_exit(t_shell *shell, void *param, t_exitflag flag, int err);
int			convert_code(char *str, int *err, int sign, long int total);
bool		str_is_num(char *str);
int			get_exit_code(char *str);
int			ft_is_exit(t_shell *shell, t_ast_node *node);//N
// int		 ft_is_exit(t_shell *shell);

//cd
void		update_env_var(t_shell *shell, char *var, char *new_value);
void		goto_home_path(t_shell *shell, int *err);
void		ft_cd(t_shell *shell, t_ast_node *node); //N
// void		ft_cd(t_shell *shell);

//echo
char		*ft_getenv(t_env_list *list, char *target);
void		ft_echo(t_shell *shell, t_ast_node *node);//N
// void		ft_echo(t_shell *shell);

//env
// void		ft_env(t_shell *shell);
void	ft_env(t_shell *shell, t_ast_node *node); //N


//export
int	var_already_exists(t_shell *shell, int *i, char *new); //N

void	print_export(t_shell *shell); //N
char		*get_var_name(const char *str);
char		*get_var_value(char *str);
void	change_var_value(t_shell *shell, int i, int j, t_ast_node *node); //N
void		ft_export(t_shell *shell, t_ast_node *node);//N
// void		ft_export(t_shell *shell);

//unset
void	ft_unset(t_shell *shell, t_ast_node *node);//N
// void		ft_unset(t_shell *shell);

//find_exec
void		ft_execute(char **cmd, char **envp, t_shell *shell);
char		*join_path_cmd(t_shell *shell, char *path, char *cmd);
char		*check_path(t_shell *shell, char **path, char *cmd);
char		*get_cmd_path(t_shell *shell, char *cmd, char **env);

//error
void		error(t_shell *shell, int exit_code);
void		ft_no_path(t_shell *shell);

//util
size_t		ft_strlen_c(const char *str, char c, bool *found);
char		*ft_strdup_c(const char *s, int *j, char c, bool *found);
int			ft_strcmp_c( const char *first, const char *second, char c);
int			ft_strcmp( const char *first, const char *second);
char		*ft_strndup(const char *s, int n);
void		ft_free_env(t_env_list **head);
void		ft_free_split(char **tab);
void		free_tokens(t_token **tokens);
char		*ft_strcpy(char *dest, const char *src);
char		*strjoin_and_free(char *s1, const char *s2);
char		*ft_strnstr_i(const char *big, const char *little, size_t len, size_t *i); //N
char		*ft_strdup2(const char *s); //N

//21 MAI : 
void	set_signals(void);

//6 Juin
char	**split_arguments(const char *s, char *set);
//// LES BUILT-INS SONT MODIFIES PLUS HAUT POUR AVOIR 
//// LA NODE EN ARGUMENT DE LA FONCTION

#endif