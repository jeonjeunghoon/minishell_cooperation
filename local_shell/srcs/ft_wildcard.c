/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 16:01:40 by seungcoh          #+#    #+#             */
/*   Updated: 2022/03/03 12:29:59 by seungcoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	wild_isin(t_list **lst, t_list *wild_str, t_token **token)
{
	t_list	*tmp;
	t_token	*new_token;

	tmp = NULL;
	new_token = NULL;
	while (wild_str)
	{
		tmp = wild_str->next;
		new_token = (t_token *)malloc(sizeof(t_token));
		new_token->double_quote = (*token)->double_quote;
		new_token->is_stream = (*token)->is_stream;
		new_token->single_quote = (*token)->single_quote;
		new_token->token = (char *)wild_str->content;
		ft_lstadd_back(lst, ft_lstnew(new_token));
		free(wild_str);
		wild_str = tmp;
	}
	ft_free(&((*token)->token));
	free(*token);
}

void	mk_ls_lst(t_list **ls_lst)
{
	int		fd;
	char	*line;

	fd = open(".ls_tmp", O_RDONLY, 0644);
	while (get_next_line(fd, &line))
		ft_lstadd_back(ls_lst, ft_lstnew(line));
	free(line);
	unlink(".ls_tmp");
}

void	exec_ls(char **argv, char **envp)
{
	int	fd;

	fd = open(".ls_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	if (execve("/bin/ls", argv, envp) == ERROR)
	{
		ft_error(strerror(errno), 1);
		exit(g_sig->exitnum);
	}
}

t_list	*get_ls_list(t_mini *mini, char **envp)
{
	int		val;
	char	**argv;
	pid_t	pid;
	t_list	*ls_lst;

	argv = (char **)malloc(sizeof(char *) * 2);
	argv[0] = (char *)malloc(sizeof(char) * 3);
	ft_strlcpy(argv[0], "ls", 3);
	argv[1] = 0;
	ls_lst = 0;
	pid = fork();
	if (pid > 0)
	{
		waitpid(pid, &val, 0x00000002);
		free(argv[0]);
		free(argv);
		if (ft_wifexited(val) == TRUE)
			exit_num_set(ft_wstopsig(val));
		mk_ls_lst(&ls_lst);
		return (ls_lst);
	}
	else if (pid == 0)
		exec_ls(argv, envp);
	return (0);
}

char	*ft_strstrf(char *str, char *to_find, int flag)
{
	int	i;
	int	j;
	int	slen;
	int	tlen;

	i = -1;
	slen = ft_strlen(str);
	tlen = ft_strlen(to_find);
	if (*to_find == 0)
		return (str);
	if (flag == 2)
		i = slen - tlen - 1;
	while (++i < slen - tlen + 1)
	{
		j = -1;
		while (++j < tlen)
			if (to_find[j] != str[i + j])
				break ;
		if (j == tlen)
			return (str + i + j);
		if (flag == 1)
			break ;
	}
	return (0);
}

void	strstr_token( t_list *wild_token, char **str, int *flag)
{
	t_list	*wild_curr;

	wild_curr = wild_token;
	while (wild_curr)
	{
		if ((*flag & 1) && wild_curr == wild_token)
			*str = ft_strstrf(*str, (char *)wild_curr->content, 1);
		else if ((*flag & 2) && !wild_curr->next)
			*str = ft_strstrf(*str, (char *)wild_curr->content, 2);
		else
			*str = ft_strstrf(*str, (char *)wild_curr->content, 0);
		if (!(*str))
			break ;
		wild_curr = wild_curr->next;
	}
}

t_list	*find_wild_str(t_list *wild_token, t_list *ls_lst, int flag)
{
	t_list	*wild_curr;
	t_list	*new_ls_lst;
	t_list	*tmp;
	char	*str;

	new_ls_lst = 0;
	while (ls_lst)
	{
		str = (char *)ls_lst->content;
		strstr_token(wild_token, &str, &flag);
		if (str)
			ft_lstadd_back(&new_ls_lst, ft_lstnew(ls_lst->content));
		tmp = ls_lst->next;
		if (!str)
			free(ls_lst->content);
		free(ls_lst);
		ls_lst = tmp;
	}
	ft_lstclear(&wild_token, free);
	return (new_ls_lst);
}

t_list	*get_wild_str(t_mini *mini, char *token)
{
	t_wild	wild;
	t_list	*wild_token;

	wild.i = 0;
	wild.start_idx = 0;
	wild_token = 0;
	wild.flag = 0;
	while (token[wild.i])
	{
		if (token[wild.i] == '\"')
			while (token[++wild.i] != '\"')
				;
		else if (token[wild.i] == '\'')
			while (token[++wild.i] != '\'')
				;
		else if (token[wild.i] == '$' && token[wild.i + 1] == '*')
			wild.i++;
		else if (token[wild.i] == '*')
		{
			if (wild.i - wild.start_idx)
			{
				wild.tmp_token.token = (char *)malloc(sizeof(char) * (wild.i - wild.start_idx + 1));
				ft_strlcpy(wild.tmp_token.token, token + wild.start_idx, wild.i - wild.start_idx + 1);
				if (refine_str(mini, &wild.tmp_token, mini->envp) != ERROR)
					ft_lstadd_back(&wild_token, ft_lstnew(wild.tmp_token.token));
				if (wild.start_idx == 0)
					wild.flag |= 1;
			}
			wild.flag |= 4;
			wild.start_idx = wild.i + 1;
		}
		wild.i++;
	}
	if (wild.start_idx && wild.i - wild.start_idx)
	{
		wild.tmp_token.token = (char *)malloc(sizeof(char) * (wild.i - wild.start_idx + 1));
		ft_strlcpy(wild.tmp_token.token, token + wild.start_idx, wild.i - wild.start_idx + 1);
		if (refine_str(mini, &wild.tmp_token, mini->envp) != ERROR)
			ft_lstadd_back(&wild_token, ft_lstnew(wild.tmp_token.token));
		wild.flag |= 2;
	}
	if (wild.flag & 4)
		return (find_wild_str(wild_token, get_ls_list(mini, mini->envp), wild.flag));
	return (0);
}
