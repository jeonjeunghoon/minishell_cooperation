/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:02:07 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/12 22:32:02 by jeunjeon         ###   ########.fr       */
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

int	ft_command(t_mini *mini, t_argv *argv)
{
	char		*cmd_path;

	cmd_path = NULL;
	if (mini_command(mini, argv->argv[0], argv) == FALSE)
	{
		if (check_cmd(mini, argv->argv[0], &cmd_path) == ERROR)
			return (0);
		mini->sig_flag = TRUE;
		ft_signal(&mini->sig_flag);
		exe_cmd(cmd_path, argv, mini->path);
		ft_free(&cmd_path);
	}
	return (0);
}

void	add_argv_back(t_argv *argv, char *redirect)
{
	char	**new_argv;
	size_t	len;
	int		i;

	len = ft_two_dimension_size(argv->argv) + 1;
	new_argv = (char **)malloc(sizeof(char *) * (len + 1));
	new_argv[len] = NULL;
	i = 0;
	while (i < len - 1 && argv->argv[i])
	{
		new_argv[i] = ft_strdup(argv->argv[i]);
		i++;
	}
	new_argv[i] = ft_strdup(redirect);
	if (new_argv[0][0] == '>' || new_argv[0][0] == '<')
	{
		ft_free(&new_argv[0]);
		new_argv[0] = ft_strdup("1");
	}
	ft_two_dimension_free(&argv->argv);
	argv->argv = new_argv;
}

void	add_argv_back2(t_argv *argv, char **strs)
{
	char	**new_argv;
	size_t	len1;
	size_t	len2;
	int		i;
	int		j;

	len1 = ft_two_dimension_size(argv->argv);
	len2 = ft_two_dimension_size(strs);
	new_argv = (char **)malloc(sizeof(char *) * (len1 + len2 + 1));
	new_argv[len1 + len2] = NULL;
	i = 0;
	while (i < len1 && argv->argv[i])
	{
		new_argv[i] = ft_strdup(argv->argv[i]);
		i++;
	}
	ft_two_dimension_free(&argv->argv);
	j = 0;
	while (i < len1 + len2 && strs[j])
	{
		new_argv[i] = ft_strdup(strs[j]);
		i++;
		j++;
	}
	argv->argv = new_argv;
}

void	create_argv_set(t_list **head, t_argv **argv)
{
	t_argv	*redirect;
	t_argv	*file;

	if ((*head)->next == NULL || ((t_argv *)(*head)->next->content)->is_stream == TRUE)
		return ;
	while ((*head)->next != NULL)
	{
		if (((t_argv *)(*head)->content)->is_redirect == TRUE)
		{
			redirect = (*head)->content;
			add_argv_back(*argv, redirect->argv[0]);
			(*argv)->is_redirect = TRUE;
			file = (*head)->next->content;
			(*argv)->file = file->argv[0];
			add_argv_back2(*argv, file->argv);
		}
		*head = (*head)->next;
	}
}

int	minishell(t_mini *mini)
{
	t_list	*head;
	t_argv	*argv;
	int		i;

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
			// 파이프를 기준으로 argv 세트를 만듭니다.
			create_argv_set(&head, &argv);
			ft_command(mini, argv);
		}
		if (head->next) // is_stream이 true이면 | 이므로 다음 argv가 존재할때 was_pipe=1로
			((t_argv *)head->next->content)->was_pipe = 1;
		head = head->next;
		argv = NULL;
	}
	return (0);
}
