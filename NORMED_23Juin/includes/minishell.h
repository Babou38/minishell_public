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

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <curses.h>
# include <errno.h>
# include "libft.h"

# define ERR_QUOTE_NOT_CLOSED "syntax error: quotes aren't closed\n"
# define ERR_SPECIAL_CHAR "syntax error: special character\n"
# define ERR_PIPE_START "syntax error: starting by pipe\n"
# define ERR_PIPE_END "syntax error: ending by pipe\n"
# define ERR_EMPTY_PIPE "syntax error: empty pipe\n"
# define ERR_INVALID_REDIRECTION "syntax error: invalid redirection\n"
# define F_DIR 1
# define F_RELATIVE 2
# define F_EXEC 3

typedef enum s_exitflag
{
	NODE,
	STR,
	NONE,
	END,
}			t_exitflag;

struct s_env_list
{
	struct s_env_list	*next;
	char				*var_name;
	char				*value;
};
typedef struct s_env_list	t_env_list;

struct s_nb
{
	int	i;
	int	j;
};
typedef struct s_nb			t_nb;

typedef struct s_command
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
	int				exit;
	int				exitcode;
	char			**env;
	char			**split_path;
	char			*str;
	bool			heredoc_quoted;
	int				heredoc_counter;
	char			*heredoc_name;
	t_env_list		*list;
	int				*pipe_fds;
	unsigned int	nb_pipes;
	char			*message;
	t_command		*cmd_list;
	int				stdin;
}	t_shell;

//main
void		signal_handler_c(int signum);
void		set_signals(void);

//prompt
char		*ft_strjoin_f(char *s1, char *s2, bool to_free);
char		*path_string(t_shell *shell);
char		*handle_incomplete_pipe(char *input);
bool		quotes_closed(const char *input);
bool		are_quotes_closed(const char *str);
char		*ft_read_input(t_shell *shell);
bool		contains_global_var(char *str, int *begin, int *end, int i);
char		*handle_quotes(t_shell *shell, char *input, bool s_is_open,
				bool d_is_open);

//heredoc
char		*get_env_value(t_shell *shell, char *var_name);
char		*read_heredoc(t_shell *shell, char *delimiter);
void		handle_heredoc(t_shell *shell, char *delimiter);
char		*extract_delimiter(t_shell *shell, char *heredoc_pos,
				size_t length);

//init_env
void		init_envp(t_shell *shell, char **envp);
int			del_envlist(t_shell **shell, char *name);
void		add_envlist(t_shell **shell, t_env_list **new_node, char *var,
				bool lone);

//pwd
char		*ft_get_cwd(t_shell *shell, int *err);
void		ft_pwd(t_shell *shell);

//exit
void		safe_exit(t_shell *shell, void *param, t_exitflag flag, int err);
int			convert_code(char *str, int *err, int sign, long int total);
bool		str_is_num(char *str);
int			get_exit_code(char *str);
int			ft_is_exit(t_shell *shell, char **v_cmd);

//cd
void		update_env_var(t_shell *shell, char *var, char *new_value);
void		goto_home_path(t_shell *shell, int *err);
void		ft_cd(t_shell *shell, char **v_cmd);

//echo
char		*ft_getenv(t_shell *shell, char *target);
void		ft_echo(t_shell *shell, char **v_cmd);

//env
void		ft_env(t_shell *shell);

//export
int			var_already_exists(t_shell *shell, int *i, char *new);
void		print_export(t_shell *shell);
char		*get_var_name(const char *str);
char		*get_var_value(char *str);
void		change_var_value(t_shell *shell, int i, int j, char **v_cmd);
void		ft_export(t_shell *shell, char **v_cmd);

//unset
void		ft_unset(t_shell *shell, char **v_cmd);

//find_exec
void		ft_execute(char **cmd, char **envp, t_shell *shell);
char		*join_path_cmd(t_shell *shell, char *path, char *cmd);
char		*get_cmd_path(t_shell *shell, char *cmd, char **path, int i);

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
char		*ft_strcpy(char *dest, const char *src);
char		*strjoin_and_free(char *s1, const char *s2);
char		*ft_strdup2(const char *s);
int			is_in_quotes(const char *str, int j);
void		printferr(char *str);
void		set_signals(void);
char		**split_arguments(const char *s, char *set);
char		*ft_strnstr_i(const char *big, const char *little, size_t len,
				size_t *i);

// 8 Juillet : death of binary tree : 
int			process_message(t_shell *shell, char *message);
bool		standardize_command(t_shell *shell, t_command *command);
size_t		count_pipes(const char *str);
int			is_in_quotes(const char *str, int j);
void		*ft_memset2(void *s, int c, size_t n);
void		free_cmd_list(t_command *cmd);
void		ft_free(void *ptr);
char		*clean_command(char *cmd);
bool		split_command(t_shell *shell, t_command *command);
char		**split_arguments(const char *s, char *set);
int			ft_isspace(int c);
bool		char_sym(const char s, const char *charset);
void		pipes_commands(t_shell *shell, t_command *command, unsigned int i);
bool		is_there_chr(char *str, char c);
void		exec_redirections(t_shell *shell, t_command *cmd, int i);
char		*next_redirection_name(t_command *cmd, int i);
char		*str_without_quotes(char *str);
bool		check_last_redirection(char *str, char c);
int			last_redirection_size(char *cmd, char c, int *start);
int			count_quotes(const char *str);
int			exec(t_shell *shell, t_command *cmd);
void		exec_errors(t_shell *shell, t_command *cmd);
void		print_errors(char *s1, char *s2, char *s3);
bool		handle_command2(t_shell *shell, t_command *cmd);
bool		handle_command3(t_shell *shell, t_command *cmd);
char		*manage_path_type(t_shell *shell, char *cmd, int *flag);
bool		is_command_valid(t_shell *shell, const char *message);
bool		is_redirection_valid(const char *str);
void		cmd_new(t_command *prev);
char		*str_without_quotes(char *str);
void		copy_quoteless(char *result, char *str);
bool		handle_command2(t_shell *shell, t_command *cmd);
void		safe_exit(t_shell *shell, void *param, t_exitflag flag, int err);
char		*get_var_name(const char *str);
char		*get_var_value(char *str);
bool		var_is_valid(char *str);
size_t		ft_get_delimiter(char *str);
void		goto_home_path(t_shell *shell, int *err);
void		exec_errors(t_shell *shell, t_command *cmd);
int			ft_strcmp(const char *first, const char *second);
int			ft_strcmp_c(const char *first, const char *second, char c);
char		*ft_strdup_c(const char *s, int *j, char c, bool *found);
size_t		ft_strlen_c(const char *str, char c, bool *found);
char		*ft_strjoin_f(char *s1, char *s2, bool to_free);
bool		env_is_empty(t_shell *shell, int len);
char		*expand_input(t_shell *shell, char *str);
bool		handle_isspace(char *str);
char		*search_heredoc(t_shell *shell, char *input, size_t i);
void		in_out_redirection(t_shell *shell, t_command *command,
				int pipe_fd, int i);
void		add_envlist(t_shell **shell, t_env_list **new_node, char *var,
				bool lone);
void		copy_without_quotes(char *word, const char *s,
				unsigned int start, unsigned int end);
int			extract_dollar_size(t_shell *shell, char *str, int begin,
				int end);

#endif