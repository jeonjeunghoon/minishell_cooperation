/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 21:49:58 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/03/01 15:40:38 by jeunjeon         ###   ########.fr       */
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

# define BASIC 1
# define EXECVE 2
# define HEREDOC 3
# define LTOR1 1 // >
# define LTOR2 2 // >>
# define RTOL1 3 // <
# define RTOL2 4 // <<
# define RTOL3 5 // <<<
# define RL 6 // <>
# define V1 7 // |
# define V2 8 // ||
# define E1 9 // &
# define E2 10 // &&
# define LV 11 // >|
# define LE 12 // >&
# define RE 13 // <&
# define WRITE 1
# define READ 0

typedef struct s_sig
{
	int		type;
	int		signum;
	int		exitnum;
}	t_sig;

t_sig	*g_sig;

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
	t_bool	is_stream;
}	t_token;

typedef struct s_argv
{
	char	**argv;
	int		redirect_stream;
	t_bool	is_redirect;
	t_bool	is_heredoc;
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
	struct termios	org_term;
	struct termios	new_term;
	char			**envp;
	char			**path;
	t_input			*input;
	int				origin_fd[2];
	int				pipe_fd[2];
	t_sig			*sig;
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
void	refine_heredoc(t_mini *mini, char **input, char **envp);
int		valid_symbol_list(char *str, int i);
t_bool	is_valid_symbol(t_mini *mini, char *str, char *prev_str, char *next_str, char **envp);
int		check_stream_symbol(t_mini *mini, t_list *token_lst, char **envp);
int		create_argv_lst(t_mini *mini, t_list **argv_lst, t_list *token_lst);
void	create_token_lst(t_mini *mini, t_list **lst, char *input, char **envp);
int		exception_handling(t_mini *mini, char *input);
int		ft_parsing(t_mini *mini);

// ft_signal
void	sig_func(int signum);
void	ft_signal(void);

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
int		go_to_home(t_mini *mini, char **envp, char *path);
int		check_path(t_mini *mini, char *path);
char	*get_path(char **envp, char *argv);
void	ft_cd(t_mini *mini, t_argv *argv);

// ft_pwd
void	ft_pwd(t_mini *mini, t_argv *argv);

// ft_export
char	*get_envname_export(char *argv);
char	**create_export_envp(char **envp, char *env);
int		is_valid_export(char *argv, int i);
int		check_export_argv(t_mini *mini, char *argv);
void	ft_export(t_mini *mini, t_argv *argv);

// ft_unset
char	**create_unset_envp(char **envp, int *position, int size);
void	get_position(int *position, char **envp, char **argv);
void	position_init(int **position, int *size, char **envp, char **argv);
int		check_unset_argv(t_mini *mini, char **argv, int *size);
void	ft_unset(t_mini *mini, t_argv *argv);

// ft_env
void	show_env(char **envp);
void	ft_env(t_mini *mini, t_argv *argv);

// ft_exit
int		check_argv(char *argv);
int		exit_exception(t_mini *mini, int argc, char **argv);
void	ft_exit(t_mini *mini, t_argv *argv);

// ft_error
void	error_symbol(t_mini *mini, char *symbol, int exit_num);
void	error_2(t_mini *mini, char *cmd, char *argv, char *msg, int exit_num);
void	error_1(t_mini *mini, char *cmd, char *msg, int exit_num);
void	ft_error(t_mini *mini, char *msg, int exit_num);

// utility
char	*get_envname(char *name);
char	*ft_getenv(char **envp, char *name);
void	exit_num_set(t_mini *mini, int num);
void	token_free(t_list *lst);
void	argv_free(t_list *lst);

// parse_utility
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
int		refine_str(t_mini *mini, t_token *token, char **envp);
int		stream_parse(t_token *token, char *input, int *end);
int		str_parse(t_token *token, char *input, int *end);
t_list	*tokenize(t_mini *mini, t_token *token, char *input, int *start, char **envp);

// tokenize_utility
void	basic_str(t_refine *refine);
void	dollar_str(t_mini *mini, t_refine *refine);
void	double_quote_str(t_mini *mini, t_refine *refine);
void	single_quote_str(t_mini *mini, t_refine *refine);
int		create_refined_str(t_mini *mini, t_refine *refine);

// tokenize_utility2
char	*get_envname_parse(char *str, int *i);
void	create_new_str(t_refine *refine, int env_len, char *tmp);
void	env_str(t_refine *refine);
t_bool	is_stream(char ch);
t_bool	str_condition(char c, t_token *token);

// tokenize_utility3
void	create_exitnum_str(t_refine *refine, char *tmp, \
							char *exit_num, int tmp_len);
void	exitnum_str(t_mini *mini, t_refine *refine);

// command_utility
void	create_path_bundle(t_mini *mini);
void	set_absolute_path(char **file_path, char *cmd);
void	create_cmdpath(t_mini *mini, char *cmd, char **cmd_path);

// command_utility2
t_bool	ft_s_isreg(int mode);
t_bool	ft_s_isdir(int mode);
int		set_redirect(t_mini *mini, t_argv *argv);
void	exe_cmd(t_mini *mini, char *cmd_path, t_argv *argv, char **envp, t_bool is_child);

// command_utility3
void	set_original_fd(t_mini *mini);
void	close_original_fd(t_mini *mini);

// ft_pipe
t_bool	when_there_is_pipe(t_mini *mini, t_argv *argv);
void    pipe_tmp_copy(t_mini *mini, t_argv *argv);

// w_utility
int		ft_wexitstatus(int stat_loc);
int		ft_wstopsig(int stat_loc);
t_bool	ft_wifexited(int stat_loc);

// redirect_utility
int		heredoc(t_mini *mini);
int		append(t_mini *mini, char *file);
int		rtol(t_mini *mini, char *file);
int		ltor(t_mini *mini, char *file);

//fd_copy
void fd_copy(int fd, int fd2);

// terminal_setting
void	terminal_setting_save(t_mini *mini);
int		terminal_setting_on(t_mini *mini);
void	terminal_setting_reset(t_mini *mini);

//ft_wildcard
t_list	*get_wild_str(t_mini *mini, t_token *token, char ** envp);

#endif
