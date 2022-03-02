/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:02:07 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/03/02 18:39:07 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	mini_command(t_mini *mini, char *cmd, t_argv *argv)
{
	if ((ft_strncmp(cmd, "echo", 5)) == 0)
		ft_echo(mini, argv);
	else if ((ft_strncmp(cmd, "cd", 3)) == 0)
		ft_cd(mini, argv);
	else if ((ft_strncmp(cmd, "pwd", 4)) == 0)
		ft_pwd(mini, argv);
	else if ((ft_strncmp(cmd, "env", 4)) == 0)
		ft_env(mini, argv);
	else if ((ft_strncmp(cmd, "export", 7)) == 0)
		ft_export(mini, argv);
	else if ((ft_strncmp(cmd, "unset", 6)) == 0)
		ft_unset(mini, argv);
	else if ((ft_strncmp(cmd, "exit", 5)) == 0)
		ft_exit(mini, argv);
	else
		return (FALSE);
	return (TRUE);
}

int	ft_command(t_mini *mini, t_argv *argv)
{
	char	*cmd_path;
	pid_t	pid;
	int		stat_loc;
	t_bool	is_child;

	cmd_path = NULL;
	pid = 0;
	is_child = FALSE;
	g_sig->type = EXECVE;
	ft_signal();
	if (argv->is_pipe == TRUE)
	{
		if (pipe(mini->pipe_fd) == ERROR)
			return (ERROR);
	}
	if (argv->is_pipe == TRUE || argv->was_pipe == TRUE)
	{
		is_child = TRUE;
		pid = fork();
	}
	if (pid > 0)
	{
		waitpid(pid, &stat_loc, 0x00000002);
		if (argv->is_pipe == TRUE)
		{
			close(mini->pipe_fd[WRITE]);
			dup2(mini->pipe_fd[READ], STDIN_FILENO);
		}
		if (argv->is_heredoc == TRUE)
			unlink(".heredoc_tmp");
		exit_num_set(ft_wexitstatus(stat_loc));
	}
	else if (pid == 0)
	{
		if (argv->is_pipe == TRUE)
		{
			dup2(mini->pipe_fd[WRITE], STDOUT_FILENO);
			close(mini->pipe_fd[READ]);
		}
		if (set_redirect(mini, argv) == ERROR)
			return (ERROR);
		if (mini_command(mini, argv->argv[0], argv) == FALSE)
		{
			create_cmdpath(mini, argv->argv[0], &cmd_path);
			exe_cmd(mini, cmd_path, argv, mini->path, is_child);
			ft_free(&cmd_path);
		}
		if (is_child == TRUE)
			exit(g_sig->exitnum);
	}
	return (0);
}

void	refine_file(t_argv *file)
{
	char	**new_argv;

	new_argv = (char **)malloc(sizeof(char *) * 2);
	new_argv[1] = NULL;
	new_argv[0] = ft_strdup(file->argv[0]);
	ft_two_dimension_free(&file->argv);
	file->argv = new_argv;
}

char	**modify_file_argv(t_argv *file)
{
	char	**cmd_argv;
	size_t	len;
	size_t	i;
	size_t	j;

	len = ft_two_dimension_size(file->argv) - 1;
	if (len == 0)
		return (NULL);
	cmd_argv = (char **)malloc(sizeof(char *) * (len + 1));
	cmd_argv[len] = NULL;
	i = 1;
	j = 0;
	while (j < len && file->argv[i] != NULL)
	{
		cmd_argv[j] = ft_strdup(file->argv[i]);
		i++;
		j++;
	}
	refine_file(file);
	return (cmd_argv);
}

char	**create_cmd(t_list **head, t_argv *argv, t_argv *file)
{
	char	**cmd;
	char	**cmd_argv;

	cmd_argv = modify_file_argv(file);
	if (argv->argv[0][0] == '>' || argv->argv[0][0] == '<')
	{
		if (cmd_argv == NULL)
		{
			cmd = (char **)malloc(sizeof(char *) * 2);
			cmd[1] = NULL;
			cmd[0] = ft_strdup("");
		}
		else
			cmd = ft_strsdup(cmd_argv);
	}
	else
	{
		if (cmd_argv == NULL)
			cmd = ft_strsdup(argv->argv);
		else
			cmd = ft_strsjoin(argv->argv, cmd_argv);
	}
	if (cmd_argv != NULL)
		ft_two_dimension_free(&cmd_argv);
	return (cmd);
}

void	finish_create(t_argv *argv, char ***cmd, char ***redirect_file)
{
	ft_two_dimension_free(&argv->argv);
	argv->argv = ft_strsjoin(*cmd, *redirect_file);
	ft_two_dimension_free(cmd);
	ft_two_dimension_free(redirect_file);
	argv->is_redirect = TRUE;
}

void	cmd_argv_exist(char ***cmd, char ***cmd_argv, char ***tmp)
{
	*tmp = ft_strsjoin(*cmd, *cmd_argv);
	ft_two_dimension_free(cmd);
	ft_two_dimension_free(cmd_argv);
	*cmd = ft_strsdup(*tmp);
	ft_two_dimension_free(tmp);
}

char	**create_first_redirect(t_list **head, t_argv *argv, t_argv *file, char ***cmd)
{
	char	**redirect_file;

	*cmd = create_cmd(head, argv, file);
	redirect_file = ft_strsjoin(((t_argv *)(*head)->content)->argv, \
								((t_argv *)(*head)->next->content)->argv);
	(*head) = (*head)->next->next;
	return (redirect_file);
}

void	combine_argvs(t_list **head, t_argv *argv, t_argv *file)
{
	char	**cmd;
	char	**redirect_file;
	char	**cmd_argv;
	char	**tmp;

	redirect_file = create_first_redirect(head, argv, file, &cmd);
	while (*head != NULL && ((t_argv *)(*head)->content)->is_redirect == TRUE)
	{
		tmp = ft_strsjoin(redirect_file, ((t_argv *)(*head)->content)->argv);
		ft_two_dimension_free(&redirect_file);
		redirect_file = ft_strsdup(tmp);
		ft_two_dimension_free(&tmp);
		(*head) = (*head)->next;
		cmd_argv = modify_file_argv((*head)->content);
		if (cmd_argv != NULL)
			cmd_argv_exist(&cmd, &cmd_argv, &tmp);
		tmp = ft_strsjoin(redirect_file, ((t_argv *)(*head)->content)->argv);
		ft_two_dimension_free(&redirect_file);
		redirect_file = ft_strsdup(tmp);
		ft_two_dimension_free(&tmp);
		(*head) = (*head)->next;
	}
	finish_create(argv, &cmd, &redirect_file);
}

void	is_redirect(t_list **head, t_argv *argv)
{
	combine_argvs(head, argv, (*head)->next->content);
	if (*head == NULL)
		return ;
	if (((t_argv *)(*head)->pre->content)->is_pipe == TRUE)
	{
		argv->is_pipe = TRUE;
		((t_argv *)(*head)->next->content)->was_pipe = TRUE;
	}
	else if (((t_argv *)(*head)->content)->is_and == TRUE)
		argv->is_and = TRUE;
	else if (((t_argv *)(*head)->content)->is_or == TRUE)
		argv->is_or = TRUE;
	return ;
}

t_bool	stream_check(t_list **head, t_argv *argv)
{
	if (((t_argv *)(*head)->content)->is_pipe == TRUE)
	{
		argv->is_pipe = TRUE;
		((t_argv *)(*head)->next->next->content)->was_pipe = TRUE;
		return (TRUE);
	}
	else if (((t_argv *)(*head)->content)->is_and == TRUE)
	{
		argv->is_and = TRUE;
		return (TRUE);
	}
	else if (((t_argv *)(*head)->content)->is_or == TRUE)
	{
		argv->is_or = TRUE;
		return (TRUE);
	}
	else if (((t_argv *)(*head)->content)->is_redirect == TRUE)
	{
		is_redirect(head, argv);
		return (TRUE);
	}
	return (FALSE);
}

void	create_argv_set(t_list **head, t_argv *argv)
{
	if ((*head)->next == NULL)
		return ;
	while ((*head)->next != NULL)
	{
		if (stream_check(head, argv) == TRUE)
			return ;
		*head = (*head)->next;
	}
}

t_bool	check_and_or(t_argv *argv)
{
	if (argv->is_and && g_sig->exitnum)
		return (TRUE);
	else if (argv->is_or && g_sig->exitnum)
		return (TRUE);
	return (FALSE);
}

int	minishell(t_mini *mini)
{
	t_list	*head;
	t_argv	*argv;
	int		i;

	head = mini->input->argv_lst;
	save_origin_fd(mini);
	while (head != NULL)
	{
		argv = head->content;
		if (argv->is_stream == FALSE)
		{
			create_argv_set(&head, argv);
			ft_command(mini, argv);
			if (head == NULL)
				break ;
			if (argv->is_and || argv->is_or)
				load_origin_fd(mini);
		}
		if (check_and_or(argv) == TRUE)
			break ;
		head = head->next;
	}
	load_origin_fd(mini);
	return (0);
}
