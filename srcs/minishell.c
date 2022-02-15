/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 15:02:07 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/16 00:45:54 by jeunjeon         ###   ########.fr       */
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
		create_cmdpath(mini, argv->argv[0], &cmd_path);
		exe_cmd(cmd_path, argv, mini->path);
		ft_free(&cmd_path);
	}
	return (0);
}

void	remove_cmd_argvs(t_argv *file)
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
	remove_cmd_argvs(file);
	return (cmd_argv);
}

char	**create_cmd(t_argv *argv, t_argv *file)
{
	char	**cmd;
	char	**cmd_argv;

	cmd = NULL;
	cmd_argv = modify_file_argv(file);
	if (cmd_argv != NULL)
	{
		if (argv->argv[0][0] == '>' || argv->argv[0][0] == '<') // > file cmd [...]
			cmd = ft_strsdup(cmd_argv);
		else // cmd [...] > file cmd_argv
			cmd = ft_strsjoin(argv->argv, cmd_argv);
		ft_two_dimension_free(&cmd_argv);
	}
	else
	{
		if (argv->argv[0][0] == '>' || argv->argv[0][0] == '<') // > file
		{
			cmd = (char **)malloc(sizeof(char *) * 2);
			cmd[1] = NULL;
			cmd[0] = ft_strdup("");
		}
		else // cmd > file
			cmd = ft_strsdup(argv->argv);
	}
	return (cmd);
}

void	combine_argvs(t_argv *argv, t_argv *redirect, t_argv *file)
{
	char	**cmd;
	char	**new_argv;
	char	**tmp;

	cmd = create_cmd(argv, file);
	tmp = ft_strsjoin(cmd, redirect->argv);
	new_argv = ft_strsjoin(tmp, file->argv);
	ft_two_dimension_free(&cmd);
	ft_two_dimension_free(&tmp);
	ft_two_dimension_free(&argv->argv);
	argv->argv = new_argv;
	argv->is_redirect = TRUE;
}

void	create_argv_set(t_list **head, t_argv **argv)
{
	if ((*head)->next == NULL || ((t_argv *)(*head)->next->content)->is_stream == TRUE)
		return ;
	while ((*head)->next != NULL)
	{
		if (((t_argv *)(*head)->content)->is_redirect == TRUE)
			combine_argvs(*argv, (*head)->content, (*head)->next->content);
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
			// i = 0;
			// while (argv->argv[++i])
			// {
			// 	if (argv->argv[i][0] != '-')
			// 	{
			// 		argv->is_input = 1;
			// 		break;
			// 	}
			// }
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
