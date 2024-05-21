/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchapuis <bchapuis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:44:49 by bchapuis          #+#    #+#             */
/*   Updated: 2024/05/21 14:16:03 by bchapuis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char *ft_read_input(void)
// {
//     char *pwd;
//     char *str;

//     pwd = getcwd(NULL, 0);
//     printf("\033[1;34mminishell(\033[0m\033[1;31m%s\033[0m", pwd);
//     str = readline("\033[1;34m)> \033[0m");
//     free(pwd);
//     add_history(str);
//     return(str);
// }

static char *strjoin_and_free(char *s1, const char *s2)
{
    char *new_str;
    char *temp_str;

    if (!s1 || !s2)
        return (NULL);
    temp_str = ft_strjoin(s1, " ");
    if (!temp_str)
        return (NULL);
    new_str = ft_strjoin(temp_str, s2);
    free(temp_str);
    free(s1);
    return (new_str);
}

char *ft_read_input(void)
{
    char *pwd;
    char *str;
    char *input;
    char *temp_input;

    pwd = getcwd(NULL, 0);
    printf("\033[1;34mminishell(\033[0m\033[1;31m%s\033[0m", pwd);
    str = readline("\033[1;34m)> \033[0m");
    free(pwd);
    add_history(str);
    input = str;
    while (input[ft_strlen(input) - 1] == '|')
    {
        temp_input = NULL;
        while (!temp_input || !*temp_input)
        {
            if (temp_input)
                free(temp_input);
            temp_input = readline("> ");
        }
        input = strjoin_and_free(input, temp_input);
        free(temp_input);
        if (!input)
            return (NULL);
    }
    return (input);
}

void init_envp(t_shell *shell, char **envp)
{
    size_t len = 0;
    size_t i = 0;
    while (envp[len])
        len++;
    shell->env = malloc((len + 1) * sizeof(char *));
    while (i < len)
    {
        shell->env[i] = ft_strdup(envp[i]);
        i++;
    }
    shell->env[len] = NULL;
}

char **ft_parsing(t_shell *shell)
{
    char **cmd;
    cmd = ft_split(shell->str, ' ');
    if (cmd == NULL)
        exit(0);
    return (cmd);
}

void printAST(t_ast_node* node)
{
    if (node == NULL)
        return;
    printf("Type: %d ", node->type);
    printf("Command: %s \n", node->value);
    if (node->type == TOKEN_CMD)
    {
        if (node->args != NULL)
        {
            int i = 0;
            while (node->args[i] != NULL)
            {
                printf("Args: %s ", node->args[i]);
                i++;
            }
        }

    }
    printf ("\n");
    printAST(node->left);
    printAST(node->right);
}

int main (int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    shell.exit = 0;
    shell.exitcode = 0;
    init_envp(&shell, envp);
    while(shell.exit == 0)
    {
        shell.str = ft_read_input();
        if (shell.str != NULL)
        {
            shell.list_token = lexer(shell.str);
            // print_tokens(shell.list_token);
            shell.ast = build_ast(shell.list_token);
            // printAST(shell.ast);
            execute_ast(shell.ast, &shell);
            free_ast(shell.ast);
            free_tokens(shell.list_token);
            free(shell.str);
        }
    }
    ft_free_split(shell.env);
    return(0);
}
