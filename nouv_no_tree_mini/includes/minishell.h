/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babou <babou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:44:54 by bchapuis          #+#    #+#             */
/*   Updated: 2024/07/17 06:44:01 by babou            ###   ########.fr       */
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

typedef struct s_command  //NEW liste chainee pour les commandes
{
	char				**v_cmd;
	char				*cmd;
	char				*input_redirection;
	char				*output_redirection;
	int					heredoc_fd;
	struct s_command	*next;
	struct s_command	*prev;
}	t_command;

typedef struct s_shell
{
	int	 exit;
	int	 exitcode;
	char	**env;
	char	**split_path;
	char	*str;
	bool	heredoc_quoted;
	int	 heredoc_counter;
	char	*heredoc_name;
	t_env_list	*list;
	t_token **list_token;
	t_ast_node *ast;
	/////NEW/////
	int				*pipe_fds;
	unsigned int	nb_pipes;
	char			*message;
	t_command		*cmd_list;
	int				stdin;
	/////////////
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
void		ft_pwd(t_shell *shell);

//exit
void		safe_exit(t_shell *shell, void *param, t_exitflag flag, int err);
int			convert_code(char *str, int *err, int sign, long int total);
bool		str_is_num(char *str);
int			get_exit_code(char *str);
//int			ft_is_exit(t_shell *shell, t_ast_node *node);//N
int		ft_is_exit(t_shell *shell, char **v_cmd);

//cd
void	update_env_var(t_shell *shell, char *var, char *new_value);
void	goto_home_path(t_shell *shell, int *err);
void	ft_cd(t_shell *shell, char **v_cmd); //N

//echo
char	*ft_getenv(t_env_list *list, char *target);
void	ft_echo(t_shell *shell, char **v_cmd);//N

//env
void	ft_env(t_shell *shell); //N


//export
int		var_already_exists(t_shell *shell, int *i, char *new); //N
void	print_export(t_shell *shell); //N
char	*get_var_name(const char *str);
char	*get_var_value(char *str);
void	change_var_value(t_shell *shell, int i, int j, char **v_cmd); //N
void	ft_export(t_shell *shell, char **v_cmd);//N

//unset
void	ft_unset(t_shell *shell, char **v_cmd);//N

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
int			is_in_quotes(const char *str, int j);//N
void		printferr(char *str);//N

//21 MAI : 
void	set_signals(void);

//6 Juin
char	**split_arguments(const char *s, char *set);
//// LES BUILT-INS SONT MODIFIES PLUS HAUT POUR AVOIR 
//// LA NODE EN ARGUMENT DE LA FONCTION


// 8 Juillet : death of binary tree : 
int		process_message(t_shell *shell, char *message);
bool	standardize_command(t_shell *shell, t_command *command);
size_t	count_pipes(const char *str);
int		is_in_quotes(const char *str, int j);
void	*ft_memset2(void *s, int c, size_t n);
void	free_cmd_list(t_command *cmd);
void	ft_free(void *ptr);
char	*clean_command(char *cmd);
bool	split_command(t_shell *shell, t_command *command);
char	**split_arguments(const char *s, char *set);
int		ft_isspace(int c);
bool	char_sym(const char s, const char *charset);
void	pipes_commands(t_shell *shell, t_command *command, unsigned int i);
bool	is_there_chr(char *str, char c);
void	exec_redirections(t_shell *shell, t_command *cmd, int i);
char	*next_redirection_name(t_command *cmd, int i);
char	*str_without_quotes(char *str);
bool	check_last_redirection(char *str, char c);
int		last_redirection_size(char *cmd, char c, int *start);
int		count_quotes(const char *str);
int		exec(t_shell *shell, t_command *cmd, int i);
void	exec_errors(t_shell *shell, t_command *cmd);
void	print_errors(char *s1, char *s2, char *s3);
bool	handle_command2(t_shell *shell, t_command *cmd);
bool	handle_command3(t_shell *shell, t_command *cmd);
bool	is_command_valid(const char *message);

# define ERR_QUOTE_NOT_CLOSED "syntax error: quotes aren't closed\n"
# define ERR_SPECIAL_CHAR "syntax error: special character\n"
# define ERR_PIPE_START "syntax error: starting by pipe\n"
# define ERR_PIPE_END "syntax error: ending by pipe\n"
# define ERR_EMPTY_PIPE "syntax error: empty pipe\n"
# define ERR_INVALID_REDIRECTION "syntax error: invalid redirection\n"

#endif