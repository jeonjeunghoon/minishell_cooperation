/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utility.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 18:51:19 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/28 09:34:48 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	basic_str(t_refine *refine)
{
	char	*ptr;
	int		len;
	int		i;

	len = ft_strlen(refine->new_str) + 1;
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	ptr[len] = '\0';
	i = 0;
	while (i < len - 1)
	{
		ptr[i] = refine->new_str[i];
		i++;
	}
	ptr[i] = refine->str[refine->i];
	ft_free(&refine->new_str);
	refine->new_str = ptr;
	refine->j = i + 1;
	refine->i++;
}

void	dollar_str(t_mini *mini, t_refine *refine)
{
	if (refine->str[refine->i + 1] == '?')
		exitnum_str(mini, refine);
	else if (refine->str[refine->i + 1] == '\0' || \
		!((refine->str[refine->i + 1] >= 'a' && refine->str[refine->i + 1] <= 'z') || \
		(refine->str[refine->i + 1] >= 'A' && refine->str[refine->i + 1] <= 'Z') || \
		(refine->str[refine->i + 1] >= '0' && refine->str[refine->i + 1] <= '9')))
		basic_str(refine);
	else if ((refine->str[refine->i + 1] == '\'' || \
			refine->str[refine->i + 1] == '\"') && refine->is_basic == TRUE)
		refine->i++;
	else if (refine->str[refine->i + 1] >= '0' && refine->str[refine->i + 1] <= '9')
	{
		if (refine->str[refine->i + 1] == '0')
		{
			char *ptr;

			ptr = ft_strjoin(refine->new_str, "-minishell");
			ft_free(&refine->new_str);
			refine->new_str = ptr;
			refine->j = ft_strlen(refine->new_str);
		}
		refine->i += 2;
	}
	else
	{
		refine->i++;
		refine->name = get_envname_parse(refine->str, &refine->i);
		refine->env = ft_getenv(refine->envp, refine->name);
		ft_free(&refine->name);
		if (refine->env == NULL)
			return ;
		else
			env_str(refine);
		refine->env = NULL;
	}
}

void	double_quote_str(t_mini *mini, t_refine *refine)
{
	refine->is_double = TRUE;
	refine->is_basic = FALSE;
	while (refine->str[refine->i] == '\"' && refine->str[refine->i] != '\0')
		refine->i++;
	while (refine->str[refine->i] != '\"' && refine->str[refine->i] != '\0')
	{
		if (refine->str[refine->i] == '$')
			dollar_str(mini, refine);
		else
			basic_str(refine);
	}
	while (refine->str[refine->i] == '\"' && refine->str[refine->i] != '\0')
		refine->i++;
	refine->is_double = FALSE;
	refine->is_basic = TRUE;
}

void	single_quote_str(t_mini *mini, t_refine *refine)
{
	int	num;

	num = 0;
	refine->is_single = TRUE;
	refine->is_basic = FALSE;
	while (refine->str[refine->i] == '\'' && refine->str[refine->i] != '\0')
	{
		refine->i++;
		num++;
	}
	while (refine->str[refine->i] != '\'' && refine->str[refine->i] != '\0')
	{
		if (num % 2 == 0 && refine->str[refine->i] == '$')
			dollar_str(mini, refine);
		else
			basic_str(refine);
	}
	while (refine->str[refine->i] == '\'' && refine->str[refine->i] != '\0')
		refine->i++;
	refine->is_single = FALSE;
	refine->is_basic = TRUE;
}

void	swung_dash_str(t_mini *mini, t_refine *refine)
{
	if ((refine->str[refine->i + 1] == '\0' || refine->str[refine->i + 1] == '/') && \
		refine->i == 0)
	{
		refine->i++;
		refine->env = ft_getenv(refine->envp, "HOME");
		if (refine->env == NULL)
			ft_error(mini, "$Home env error", 1);
		else
			env_str(refine);
	}
	else
		basic_str(refine);
}

int	wildcard_str(t_mini *mini, t_refine *refine)
{
	char	**strs;
	char	*join_str;
	int		join_len;
	char	*tmp_str;
	int		i;
	int		j;
	char	**new_strs;
	int		new_len;

	strs = NULL;
	while (refine->str[refine->i] != '\0')
	{
		i = 0;
		if (refine->str[refine->i] == '*')
		{
			// if (strs == NULL) // 반복문 첫 실행
			// {
				/*

				[...]* 로 생성 가능한 모든 문자열을 strs이중 문자열로 만듬 (함수)
				// join_len = refine->i - 1;
				// join_str = (char *)malloc(sizeof(char) * (join_len + 1));
				// join_str[join_len] = '\0';
				// i = 0;
				// while (i < join_len)
				// {
					// join_str[i] = refine->str[i];
					// i++;
				// }
				
				*/

				/*
				join_str로 시작되는 모든 파일 목록 문자열을 strs에 할당함 (함수)

			
				*/
				// refine->i++;
			// }
			// else
			// {
				/*

				아래에서 수정된 strs의 문자열 중 제거할 건 제거하고 새로 할당한다. (함수)
				// new_len = 0;
				// i = 0;
				// while (strs[i] != NULL) // 새로운 2중포인터를 만들기 위해 유효한 문자열의 개수를 센다.
				// {
					// if (strs[i][0] != '\0')
						// new_len++;
					// i++;
				// }
				// if (new_len == 0) // 유효한 문자열이 없을 경우 종료
				// {
					// refine->new_str = ft_strdup(refine->str);
					// refine->i++;
					// return (ERROR);
				// }
				// new_strs = (char **)malloc(sizeof(char *) * (new_len + 1));
				// new_strs[new_len] = NULL;
				// i = 0;
				// j = 0;
				// while (i < new_len)
				// {
					// if (strs[j][0] == '\0')
						// j++;
					// else
					// {
						// new_strs[i] = ft_strdup(strs[j])
						// i++;
						// j++;
					// }
				// }
				// ft_two_demension_free(&strs);
				// strs = ft_strsdup(new_strs);
				// ft_two_demension_free(&new_strs);
				// refine->i += ft_strlen(join_str);
				// ft_free(&join_str);
			// }

				*/
		}
		else
		{
			/*
			
			와일드카드가 나오기 전까지 혹은 널문자 전까지의 join문자열을 생성 (함수)
			// i = refine->i;
			// while (strs[i] != '*' && strs[i] != '\0')
				// i++;
			// join_len = refine->i - i;
			// join_str = (char *)malloc(sizeof(char) * (join_len + 1));
			// join_str[join_len] = '\0';
			// i = 0;
			// while (i < join_len)
			// {
				// join_str[i] = strs[refine->i];
				// i++;
				// refine->i++;
			// }
			// refine->i -= i;

			*/
			/*
			
			와일드카드로 생성된 모든 문자열들과 join문자열을 ft_strjoin으로 붙임 (함수)
			// i = 0;
			// while (strs[i] != NULL)
			// {
				// tmp_str = ft_strjoin(strs[i]);
				// if ((open()) == ERROR)
					// tmp_str[0] = '\0';
				// ft_free(&(strs[i]));
				// strs[i] = ft_strdup(tmp_str);
				// ft_free(tmp_str);
				// i++;
			// }
			
			*/
		}
	}
	// 생성된 strs의 갯수 만큼 토큰을 만들어 토큰->토큰에 str을 할당한다.
	// 만들어진 토큰들을 토큰 리스트로 만든다.
	// 만들어진 리스트를 지금 포인팅하고 있는 토큰 대신 넣는다.
	// 리스트 마지막 노드의 next포인터를 포인팅하던 토큰의 포인터로 넣는다.
	return (0);
}

int	create_refined_str(t_mini *mini, t_refine *refine)
{
	int	len;
	int	i;

	len = ft_strlen(refine->str);
	refine->new_str = (char *)malloc(sizeof(char) * (len + 1));
	refine->new_str[len] = '\0';
	i = 0;
	while (i < len)
		refine->new_str[i++] = '\0';
	while (refine->str[refine->i])
	{
		if (refine->str[refine->i] == '*')
		{
			if (wildcard_str(mini, refine) == ERROR)
				return (0);
		}
		else if (refine->str[refine->i] == '~')
			swung_dash_str(mini, refine);
		else if (refine->str[refine->i] == '\'')
			single_quote_str(mini, refine);
		else if (refine->str[refine->i] == '\"')
			double_quote_str(mini, refine);
		else if (refine->str[refine->i] == '$')
			dollar_str(mini, refine);
		else
			basic_str(refine);
	}
	if (refine->new_str[0] == '\0')
		return (ERROR);
	return (0);
}
