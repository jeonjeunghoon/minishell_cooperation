/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:02:07 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/07 18:07:03 by jeunjeon         ###   ########.fr       */
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

int	ft_command(t_mini *mini, char **argv)
{
	char		*cmd_path;

	cmd_path = NULL;
	if (argv[0][0] == '\0')
		return (0);
	if (mini_command(mini, argv[0], argv) == FALSE)
	{
		if (check_cmd(mini, argv[0], &cmd_path) == ERROR)
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
			ft_command(mini, argv->argv);
		head = head->next;
		argv = NULL;
	}
	return (0);
}
