/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 21:49:06 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/23 15:14:31 by seungcoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	clear_resource(t_mini *mini)
{
	ft_two_dimension_free(&(mini->path));
	token_free(mini->input->token_lst);
	argv_free(mini->input->argv_lst);
	ft_free(&mini->input->user_input);
}

void	minishell_init(t_mini *mini)
{
	mini->path = NULL;
	mini->input->token_lst = NULL;
	mini->input->argv_lst = NULL;
	mini->input->user_input = NULL;
	mini->sig_flag = FALSE;
}

int	memory_allocation(t_mini **mini, char **envp)
{
	int	i;
	int	size;

	(*mini) = (t_mini *)malloc(sizeof(t_mini));
	size = ft_two_dimension_size(envp);
	(*mini)->envp = (char **)malloc(sizeof(char *) * (size + 1));
	(*mini)->envp[size] = NULL;
	i = -1;
	while (envp[++i])
		(*mini)->envp[i] = ft_strdup(envp[i]);
	(*mini)->input = (t_input *)malloc(sizeof(t_input));
	if (tcgetattr(STDIN_FILENO, &((*mini)->term)) == -1)
		return (ERROR);
	((*mini)->term).c_lflag &= ~(ECHOCTL);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &((*mini)->term)) == -1)
		return (ERROR);
	if ((*mini) == NULL || (*mini)->input == NULL)
		return (ERROR);
	return (0);
}

int	main(int argc, const char **argv, char **envp)
{
	t_mini	*mini;
	int		error_fd;
	
	if (argc != 1 || argv == NULL)
		return (0);
	if (memory_allocation(&mini, envp) == ERROR)
	{
		ft_error("allocation error", 1);
		exit(g_exit_state);
	}
	while (TRUE)
	{
		minishell_init(mini);
		ft_signal(BASIC);
		if (ft_prompt(mini) == ERROR)
		{
			ft_error("prompt error", 1);
			exit(g_exit_state);
		}
		if (mini->input->user_input[0] != '\0')
		{
			if (ft_parsing(mini) != ERROR)
			{
				minishell(mini);
				error_fd = open(".error_tmp", O_RDONLY, 0644);
				if (error_fd != -1)
				{
					fd_copy(error_fd, 2);
					close(error_fd);
					unlink(".error_tmp");
				}
			}
		}
		clear_resource(mini);
	}
	return (0);
}
