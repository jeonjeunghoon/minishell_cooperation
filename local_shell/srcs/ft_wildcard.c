/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 16:01:40 by seungcoh          #+#    #+#             */
/*   Updated: 2022/03/02 17:33:16 by jeunjeon         ###   ########.fr       */
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

t_list	*get_ls_list(t_mini *mini, char **envp)
{
	int		fd;
	char	**argv;
	char	*line;
	pid_t	pid;
	int		stat_loc;
	t_list	*ls_lst;
	
	argv = (char **)malloc(sizeof(char *) * 2);
	argv[0] = (char *)malloc(sizeof(char) * 3);
	ft_strlcpy(argv[0], "ls", 3);
	argv[1] = 0;
	ls_lst = 0;
	pid = fork();
	if (pid > 0)
	{
		waitpid(pid, &stat_loc, 0x00000002);
		free(argv[0]);
		free(argv);
		if (ft_wifexited(stat_loc) == TRUE)
			exit_num_set(ft_wstopsig(stat_loc));
		fd = open(".ls_tmp", O_RDONLY, 0644);
		while (get_next_line(fd, &line))
			ft_lstadd_back(&ls_lst, ft_lstnew(line));
		free(line);
		unlink(".ls_tmp");
		return ls_lst;
	}
	else if (pid == 0)
	{
		fd = open(".ls_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		if (execve("/bin/ls", argv, envp) == ERROR)
		{
			ft_error(strerror(errno), 1);
			exit(g_sig->exitnum);
		}
	}
	return 0;
}	

char	*ft_strstrf(char *str, char *to_find, int flag)
{
	int i;
	int j;
	int slen;
	int tlen;

	i = 0;
	slen = ft_strlen(str);
	tlen = ft_strlen(to_find);
	if (*to_find == 0)
		return (str);
	if (flag == 2)
		i = slen - tlen;
	while (i < slen - tlen + 1)
	{
		j = 0;
		while (j < tlen)
		{
			if (to_find[j] != str[i + j])
				break ;
			j++;
		}
		if (j == tlen)
			return (str + i + j);
		if (flag == 1)
			break;
		i++;
	}
	return (0);
}

t_list	*find_wild_str(t_list  *wild_token, t_list *ls_lst, int flag)
{
	t_list	*wild_curr;
	t_list	*new_ls_lst;
	t_list	*tmp;
	char	*str;

	new_ls_lst = 0;
	while (ls_lst)
	{
		wild_curr = wild_token;
		str = (char *)ls_lst->content;
		while (wild_curr)
		{
			//첫 토큰와 마지막 토큰 예외 처리 필요
			if ((flag & 1) && wild_curr == wild_token)
				str = ft_strstrf(str, (char *)wild_curr->content, 1);
			else if ((flag & 2) && !wild_curr->next)
				str = ft_strstrf(str, (char *)wild_curr->content, 2);
			else
				str = ft_strstrf(str, (char *)wild_curr->content, 0);
			if (!str)
				break;
			wild_curr = wild_curr->next;
		}
		if (str)
			ft_lstadd_back(&new_ls_lst, ft_lstnew(ls_lst->content));
		tmp = ls_lst->next;
		if (!str)
			free(ls_lst->content);
		free(ls_lst);
		ls_lst = tmp;
	}
	while (wild_token)
	{
		tmp = wild_token->next;
		free(wild_token->content);
		free(wild_token);
		wild_token = tmp;
	}
	return new_ls_lst;
}

t_list	*get_wild_str(t_mini *mini, t_token *token, char ** envp)
{
    int     i;
    int     start_idx;
	int		flag;
    t_token *tmp_token;
    t_list  *wild_token;
	t_list	*ls_lst;
	t_list	*curr;

    i = 0;
    start_idx = 0;
    wild_token = 0;
	flag = 0;
    tmp_token = (t_token *)malloc(sizeof(t_token));
    while (token->token[i])
    {
        if (token->token[i] == '\"')
            while (token->token[++i] != '\"')
                ;
        else if (token->token[i] == '\'')
            while (token->token[++i] != '\'')
                ;
        else if (token->token[i] == '$' && token->token[i + 1] == '*')
            i++;
        else if (token->token[i] == '*')
        {
            if (i - start_idx)
            {
				tmp_token->token = (char *)malloc(sizeof(char) * (i - start_idx + 1));
				ft_strlcpy(tmp_token->token, token->token + start_idx, i - start_idx + 1);
				if (refine_str(mini, tmp_token, envp) == ERROR)
					return ((t_list *)ERROR);
				ft_lstadd_back(&wild_token, ft_lstnew(tmp_token->token));
				if (start_idx == 0)
					flag |= 1;
			}
            start_idx = i + 1;
        }
        i++;
    }
	if (start_idx && i - start_idx)
	{
		tmp_token->token = (char *)malloc(sizeof(char) * (i - start_idx + 1));
		ft_strlcpy(tmp_token->token, token->token + start_idx, i - start_idx + 1);
		if (refine_str(mini, tmp_token, envp) == ERROR)
			return ((t_list *)ERROR);
		ft_lstadd_back(&wild_token, ft_lstnew(tmp_token->token));
		flag |= 2;
	}
	curr = wild_token;
	free(tmp_token);
	
	// printf("%d\n", flag);
	// while(curr)
	// {
	// 	printf("[%s]\n", (char *)curr->content);
	// 	curr = curr->next;
	// }

	//ls를 통해서 현재 디렉토리 파일 목록 문자열화
	if (wild_token || (token->token[0] == '*' && token->token[1] == 0))
		return (find_wild_str(wild_token, get_ls_list(mini, envp), flag));
    return (0);
}