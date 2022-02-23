/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 21:49:58 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/23 15:04:33 by seungcoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <sys/errno.h>
# include <signal.h>
# include <dirent.h>
# include <term.h>
# include <curses.h>

# define BASIC 0
# define EXECVE 1
# define HEREDOC 2

int			g_exit_state;

typedef struct s_refine
{
	char	*new_str;
	char	*str;
	char	**envp;
	char	*name;
	char	*env;
	int		i;
	int		j;
	t_bool	is_basic;
	t_bool	is_single;
	t_bool	is_double;
}	t_refine;

typedef struct s_token
{
	char	*token;
	t_bool	single_quote;
	t_bool	double_quote;
}	t_token;

typedef struct s_argv
{
	char	**argv;
	int		redirect_stream;
	t_bool	is_redirect;
	t_bool	is_stream;
	t_bool	was_pipe; //이전 argv에 pipe가 있는가? 
	t_bool	is_pipe; //다음 argv에 pipe가 있는가? "du -h | sort -nr" 이면 du는 was = 0 is = 1, sort는 was = 1, is = 0
	// t_bool	is_input; //명령어의 input이 존재하는가? "ls -a | sort -nr a.txt"이면 ls 출력x, ls 내용과 무관하게 a.txt를 sort 이때 sort의 is_input은 1
	t_bool	is_and; // &&
	t_bool	is_or; // ||
	t_bool	is_wildcard; // *
	t_bool	hav_cmd;
}	t_argv;

typedef struct s_input
{
	t_list	*token_lst;
	t_list	*argv_lst;
	char	*user_input;
}	t_input;

typedef struct s_mini
{
	char			**envp;
	char			**path;
	t_input			*input;
	struct termios	term;
	t_bool			sig_flag;
}	t_mini;

// main
void	clear_resource(t_mini *mini);
void	minishell_init(t_mini *mini);
int		memory_allocation(t_mini **mini, char **envp);
int		main(int argc, const char **argv, char **envp);

// ft_prompt
char	*get_prompt(char *locate, char **envp);
char	*get_locate(void);
int		ft_prompt(t_mini *mini);

// ft_parsing
int		check_stream_symbol(t_list *token_lst);
int		create_argv_lst(t_list **argv_lst, t_list *token_lst);
void	create_token_lst(t_list **lst, char *input, char **envp);
int		exception_handling(char *input);
int		ft_parsing(t_mini *mini);

// ft_signal
void	sigint_func(int sig);
void	sig_func(int sig);
void	ft_signal(int sig_flag);

// minishell
int		mini_command(t_mini *mini, char *cmd, t_argv *argv);
int		ft_command(t_mini *mini, t_argv *argv);
int		set_stream(t_list *head);
int		minishell(t_mini *mini);

// ft_echo
void	print_msg(char **envp, char **argv, int start_ptr, int n_flag);
int		n_option(char **argv, int *start_ptr);
void	ft_echo(t_mini *mini, t_argv *argv);

// ft_cd
void	set_env_cd(t_mini *mini, char *old_pwd);
int		go_to_home(char **envp, char *path);
int		check_path(char *path);
char	*get_path(char **envp, char *argv);
void	ft_cd(t_mini *mini, t_argv *argv);

// ft_pwd
void	ft_pwd(t_argv *argv);

// ft_export
char	*get_envname_export(char *argv);
int		is_valid_export(char *argv, int i);
char	**create_export_envp(char **envp, char *env);
int		check_export_argv(char *argv);
void	ft_export(t_mini *mini, t_argv *argv);

// export_utility.c
void	set_envp(char ***envp);
void	create_export_tmp(char **envp);

// ft_unset
char	**create_unset_envp(char **envp, int *position, int size);
void	get_position(int *position, char **envp, char **argv);
void	position_init(int **position, int *size, char **envp, char **argv);
int		check_unset_argv(char **argv, int *size);
void	ft_unset(t_mini *mini, t_argv *argv);

// ft_env
void	show_env(char **envp);
void	ft_env(t_mini *mini, t_argv *argv);

// ft_exit
int		check_argv(char *argv);
int		exit_exception(int argc, char **argv);
void	ft_exit(t_argv *argv);

// ft_error
void	error_symbol(char symbol, int exit_num);
void	error_symbol2(char *symbol, int exit_num);
void	error_2(char *cmd, char *argv, char *msg, int exit_num);
void	error_1(char *cmd, char *msg, int exit_num);
void	ft_error(char *msg, int exit_num);

// utility
char	*get_envname(char *name);
char	*ft_getenv(char **envp, char *name);
void	exit_num_set(int num);
void	token_free(t_list *lst);
void	argv_free(t_list *lst);

// parse_utility
t_bool	is_valid_symbol(char *str);
int		stream_flag_str(t_token *token);
void	token_init(t_token *token);
int		create_stream(t_argv **stream, t_token *token, t_list **argv_lst);
void	create_argv(t_argv **argv, t_list *token_lst, \
					t_list **argv_lst, int size);

// parse_utility2
void	exception_utility(char c, t_bool *sin, t_bool *dou);
void	argv_lst_init(t_argv **str, t_argv **stream, int *size);
void	argv_init(t_argv *argv);

// tokenize
void	refine_init(t_refine *refine);
int		refine_str(t_token *token, char **envp);
int		stream_parse(t_token *token, char *input, int *end);
int		str_parse(t_token *token, char *input, int *end);
int		tokenize(t_token *token, char *input, int *start, char **envp);

// tokenize_utility
void	basic_str(t_refine *refine);
void	dollar_str(t_refine *refine);
void	double_quote_str(t_refine *refine);
void	single_quote_str(t_refine *refine);
int		create_refined_str(t_refine *refine);

// tokenize_utility2
char	*get_envname_parse(char *str, int *i);
void	create_new_str(t_refine *refine, int env_len, char *tmp);
void	env_str(t_refine *refine);
t_bool	stream_condition(char c);
t_bool	str_condition(char c, t_token *token);

// tokenize_utility3
void	create_exitnum_str(t_refine *refine, char *tmp, \
							char *exit_num, int tmp_len);
void	exitnum_str(t_refine *refine);

// stream_utility
void	heredoc_redirect(t_list *head, t_bool is_error);
void	r_to_l_redirect(t_list *head, char *argv, t_bool is_error);
void	append_redirect(t_list *head, t_bool is_error);
void	l_to_r_redirect(t_list *head, char *argv, t_bool is_error);

// stream_utility2
void	double_ampersand(t_list *head, t_bool is_error);
void	double_verticalbar(t_list *head, t_bool is_error);
void	verticalbar(t_list *head, char *argv, t_bool is_error);

// command_utility
void	create_path_bundle(t_mini *mini);
void	set_absolute_path(char **file_path, char *cmd);
void	create_cmdpath(t_mini *mini, char *cmd, char **cmd_path);

// command_utility2
t_bool	ft_s_isreg(int mode);
t_bool	ft_s_isdir(int mode);
int		set_redirect(t_argv *argv);
void	exe_cmd(char *cmd_path, t_argv *argv, char **envp);

// command_utility3
void	set_original_fd(t_argv *argv, int *original_fd);
void	close_original_fd(t_argv *argv, int *original_fd);

// ft_pipe
void    when_there_is_pipe(t_argv *argv);
void    pipe_tmp_copy(t_argv *argv);

// w_utility
int		ft_wexitstatus(int stat_loc);
int		ft_wstopsig(int stat_loc);
t_bool	ft_wifexited(int stat_loc);

// redirect_utility
int		heredoc(char *delimiter);
int		append(char *file);
int		rtol(char *file);
int		ltor(char *file);

//fd_copy
void fd_copy(int fd, int fd2);

#endif
