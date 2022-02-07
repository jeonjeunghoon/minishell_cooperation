/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:02:07 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/07 20:56:58 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	mini_command(t_mini *mini, char *cmd, char **argv)
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

int	ft_command(t_mini *mini, t_argv *argv)
{
	char		*cmd_path;

	cmd_path = NULL;
	if (argv->argv[0][0] == '\0')
		return (0);
	if (mini_command(mini, argv->argv[0], argv->argv) == FALSE)
	{
		if (check_cmd(mini, argv->argv[0], &cmd_path) == ERROR)
			return (0);
		exe_cmd(cmd_path, argv, mini->path, mini->sig_flag);
		if (cmd_path != NULL)
		{
			free(cmd_path);
			cmd_path = NULL;
		}
	}
	return (0);
}

int	set_stream(t_list *head)
{
	char	*argv;
	t_bool	is_error;

	is_error = FALSE;
	while (head != NULL)
	{
		argv = (((t_argv *)(head->content))->argv)[0];
		if (argv[0] == '>')
			l_to_r_redirect(head, argv, is_error);
		else if (argv[0] == '<')
			r_to_l_redirect(head, argv, is_error);
		else if (argv[0] == '|')
			verticalbar(head, argv, is_error);
		else if (argv[0] == '&' && argv[1] == '&')
			double_ampersand(head, is_error);
		if (is_error == TRUE)
			return (ERROR);
		head = head->next;
	}
	return (0);
}

int	minishell(t_mini *mini)
{
	t_list	*head;
	t_argv	*argv;
	int		i;

	head = mini->input->argv_lst;
	// if (set_stream(head) == ERROR)
	// {
	// 	exit_num_set(1); // 수정해야함
	// 	return (0);
	// }
	head = mini->input->argv_lst;
	while (head != NULL)
	{
		argv = head->content;
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
			ft_command(mini, argv);
		}
		else if (head->next) // is_stream이 true이면 | 이므로 다음 argv가 존재할때 was_pipe=1로
			((t_argv *)head->next->content)->was_pipe = 1;
		head = head->next;
		argv = NULL;
	}
	return (0);
}
