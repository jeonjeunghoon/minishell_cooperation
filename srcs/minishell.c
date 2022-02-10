/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:02:07 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/10 23:47:50 by jeunjeon         ###   ########.fr       */
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
		ft_pwd(argv);
	else if ((ft_strncmp(cmd, "env", 4)) == 0)
		ft_env(mini, argv);
	else if ((ft_strncmp(cmd, "export", 7)) == 0)
		ft_export(mini, argv);
	else if ((ft_strncmp(cmd, "unset", 6)) == 0)
		ft_unset(mini, argv);
	else if ((ft_strncmp(cmd, "exit", 5)) == 0)
		ft_exit(argv);
	else
		return (FALSE);
	return (TRUE);
}

int	ft_command(t_mini *mini, t_argv *argv, t_argv *redirect, t_argv *file)
{
	char		*cmd_path;

	cmd_path = NULL;
	if (mini_command(mini, argv->argv[0], argv) == FALSE)
	{
		if (check_cmd(mini, argv->argv[0], &cmd_path) == ERROR)
			return (0);
		mini->sig_flag = TRUE;
		ft_signal(&mini->sig_flag);
		exe_cmd(cmd_path, argv, mini->path, redirect, file);
		ft_free(&cmd_path);
	}
	return (0);
}

t_bool	is_redirect(t_argv *argv)
{
	if (argv->is_ltor == TRUE || argv->is_rtol == TRUE || argv->is_append == TRUE || argv->is_heredoc == TRUE)
		return (TRUE);
	return (FALSE);
}

void	check_redirect(t_list **head, t_argv **argv, t_argv **redirect, t_argv **file)
{
	if ((*head)->next == NULL)
		return ;
	// if argv 가 커맨드인지 리다이렉션인지 체크
		// if 커맨드라면 다음 argv가 리다이렉션인지 체크
			// if 리다이렉션이 아니라면 redirect = NULL; file = NULLl return ;
		// else if 리다이렉션이라면 free(argv->argv); argv->argv = NULL;
	// file = 리다이렉션argv->argv;
	if (is_redirect(*argv) == FALSE)
	{
		*redirect = (*head)->next->content;
		if (is_redirect(*redirect) == FALSE)
		{
			*redirect = NULL;
			*file = NULL;
			return ;
		}
		*file = (*head)->next->next->content;
		*head = (*head)->next->next;
	}
	else if (is_redirect(*argv) == TRUE)
	{
		*redirect = *argv;
		ft_two_dimension_free(&((*argv)->argv));
		*argv = NULL;
		*file = (*head)->next->content;
		*head = (*head)->next;
	}
}

int	minishell(t_mini *mini)
{
	t_list	*head;
	t_argv	*argv;
	t_argv	*redirect;
	t_argv	*file;
	int		i;

	head = mini->input->argv_lst;
	while (head != NULL)
	{
		argv = head->content;
		redirect = NULL;
		file = NULL;
		if (argv->is_stream == FALSE)
		{
			// 이번 명령어의 인자들중 '-'가 아닌 인자, 즉 입력 파일 이름이 존재하는지 체크
			i = 0;
			while (argv->argv[++i])
			{
				if (argv->argv[i][0] != '-')
				{
					argv->is_input = 1;
					break;
				}
			}
			// 파이프를 기준으로 argv 셋이 들어갑니다.
			// 커맨드가 첫 번째 argv 일 때 (커맨드가 있는 리다이렉션) argv = 커맨드
			// 리다이렉션이 첫 번째 argv 일 때 (커맨드가 없는 리다이렉션) argv = NULL
			check_redirect(&head, &argv, &redirect, &file);
			ft_command(mini, argv, redirect, file);
		}
		else if (head->next) // is_stream이 true이면 | 이므로 다음 argv가 존재할때 was_pipe=1로
			((t_argv *)head->next->content)->was_pipe = 1;
		head = head->next;
		argv = NULL;
	}
	return (0);
}
