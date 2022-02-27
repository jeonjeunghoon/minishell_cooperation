/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:02:07 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/27 20:40:07 by jeunjeon         ###   ########.fr       */
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
		exit_num_set(mini, ft_wexitstatus(stat_loc));
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
			exit(mini->sig->exitnum);
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

	cmd = NULL;
	cmd_argv = modify_file_argv(file);
	// if (cmd_argv != NULL)
	// {
	// 	if (argv->argv[0][0] == '>' || argv->argv[0][0] == '<') // > file cmd [...]
	// 		cmd = ft_strsdup(cmd_argv);
	// 	else // cmd [...] > file cmd_argv
	// 		cmd = ft_strsjoin(argv->argv, cmd_argv);
	// }
	// else if (cmd_argv == NULL)
	// {
	// 	if (argv->argv[0][0] == '>' || argv->argv[0][0] == '<') // > file
	// 	{
	// 		cmd = (char **)malloc(sizeof(char *) * 2);
	// 		cmd[1] = NULL;
	// 		cmd[0] = ft_strdup("");
	// 	}
	// 	else // cmd > file
	// 		cmd = ft_strsdup(argv->argv);
	// }

	if (argv->argv[0][0] == '>' || argv->argv[0][0] == '<') // (>) (file [cmd ...])
	{
		if (cmd_argv == NULL) // (>) (file)
		{
			cmd = (char **)malloc(sizeof(char *) * 2);
			cmd[1] = NULL;
			cmd[0] = ft_strdup("");
		}
		else // (>) (file cmd [...])
			cmd = ft_strsdup(cmd_argv);
	}
	else // (cmd [...]) (>) (file [cmd ...])
	{
		if (cmd_argv == NULL) // cmd [...] > file
			cmd = ft_strsdup(argv->argv);
		else // (cmd [...]) (>) (file cmd_argv)
			cmd = ft_strsjoin(argv->argv, cmd_argv);
	}
	if (cmd_argv != NULL)
		ft_two_dimension_free(&cmd_argv);
	return (cmd);
}

// cmd argument redirect file [r f ...]

void	combine_argvs(t_list **head, t_argv *argv, t_argv *redirect, t_argv *file)
{
	char	**cmd;
	char	**redirect_file;
	char	**cmd_argv;
	char	**tmp;

	cmd = create_cmd(head, argv, file);
	redirect_file = ft_strsjoin(((t_argv *)(*head)->content)->argv, ((t_argv *)(*head)->next->content)->argv);
	(*head) = (*head)->next->next;
	while (*head != NULL && ((t_argv *)(*head)->content)->is_redirect == TRUE)
	{
		tmp = ft_strsjoin(redirect_file, ((t_argv *)(*head)->content)->argv);
		ft_two_dimension_free(&redirect_file);
		redirect_file = ft_strsdup(tmp);
		ft_two_dimension_free(&tmp);
		(*head) = (*head)->next;
		cmd_argv = modify_file_argv((*head)->content);
		if (cmd_argv != NULL)
		{
				tmp = ft_strsjoin(cmd, cmd_argv);
				ft_two_dimension_free(&cmd);
				ft_two_dimension_free(&cmd_argv);
				cmd = ft_strsdup(tmp);
				ft_two_dimension_free(&tmp);
		}
		tmp = ft_strsjoin(redirect_file, ((t_argv *)(*head)->content)->argv);
		ft_two_dimension_free(&redirect_file);
		redirect_file = ft_strsdup(tmp);
		ft_two_dimension_free(&tmp);
		(*head) = (*head)->next;
	}
	ft_two_dimension_free(&argv->argv);
	argv->argv = ft_strsjoin(cmd, redirect_file);
	ft_two_dimension_free(&cmd);
	ft_two_dimension_free(&redirect_file);
	argv->is_redirect = TRUE;
}

void	create_argv_set(t_list **head, t_argv *argv)
{
	t_bool	in_redirect;

	if ((*head)->next == NULL)
		return ;
	while ((*head)->next != NULL)
	{
		in_redirect = FALSE;
		if (((t_argv *)(*head)->content)->is_pipe == TRUE)
		{
			argv->is_pipe = TRUE;
			((t_argv *)(*head)->next->next->content)->was_pipe = TRUE;
			return ;
		}
		else if (((t_argv *)(*head)->content)->is_and == TRUE)
		{
			argv->is_and = TRUE;
			return ;
		}
		else if (((t_argv *)(*head)->content)->is_or == TRUE)
		{
			argv->is_or = TRUE;
			return ;
		}
		else if (((t_argv *)(*head)->content)->is_redirect == TRUE)
		{
			combine_argvs(head, argv, (*head)->content, (*head)->next->content);
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
		*head = (*head)->next;
	}
}

int	minishell(t_mini *mini)
{
	t_list	*head;
	t_argv	*argv;
	int		i;

	/*head = mini->input->argv_lst;
	printf("[argv_list]\n");
	printf("head:%p\n", head);
	while(head!=NULL)
	{	
		argv = head->content;
		char **ptr = ((t_argv *)head->content)->argv;
		int j = 0;
		while(ptr[j])
			printf("%s %d %d\n", ptr[j++], argv->is_pipe, argv->is_or);
		printf("\n");
		head = head->next;
	}*/
	head = mini->input->argv_lst;
	set_original_fd(mini);
	while (head != NULL)
	{
		argv = head->content;
		if (argv->is_stream == FALSE)
		{
			create_argv_set(&head, argv);

			// printf("[after create_argv_set]\n");
			// printf("head: %s\n", ((t_argv *)head->content)->argv[0]);
			// char **ptr = argv->argv;
			// int j = 0;
			// while(ptr[j])
			// 	printf("%s %d %d\n", ptr[j++], argv->is_pipe, argv->was_pipe);
			// printf("\n");

			ft_command(mini, argv);
			if (head == NULL)
				break ;
			if (argv->is_and || argv->is_or)
			{
				close_original_fd(mini);
				set_original_fd(mini);
			}
			// if (head->next)
			// 	((t_argv *)head->next->content)->hav_cmd = 1;
		}
		// else if(!argv->hav_cmd)
		// {
		// 	error_symbol(mini, argv->argv[0], 2);
		// 	break;
		// }
		if (argv->is_and)
		{
			if (mini->sig->exitnum)
				break;
		}
		else if (argv->is_or)
		{
			if (!mini->sig->exitnum)
				break;
		}
		/*if (argv->is_pipe && !head->next->next)
		{
			//에러메세지 무언가 필요할듯
			printf("aaaa\n");
		}
		if (argv->is_and && !head->next->next)
			//에러메세지 무언가 필요할듯
		if (argv->is_or && !head->next->next)
			//에러메세지 무언가 필요할듯*/
		head = head->next;
		argv = NULL;
	}
	close_original_fd(mini);
	return (0);
}
