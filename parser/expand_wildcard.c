/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouturi <lcouturi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by lcouturi          #+#    #+#             */
/*   Updated: 2024/01/20 20:01:00 by lcouturi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	get_arg_num_loop(char **args, int *i, int *n)
{
	char	**files;
	char	**split;

	i[0] = -1;
	while (args[i[2]][++i[0]])
	{
		quote_check(args[i[2]], i);
		if (!i[3] && !i[4] && args[i[2]][i[0]] == '*')
		{
			files = get_file_list(args[i[2]][0] == '.');
			i[1] = -1;
			split = ft_split(args[i[2]], '*');
			while (files[++i[1]])
				match(args[i[2]], split, files, i[1]);
			strarrfree(split);
			i[1] = -1;
			while (files[++i[1]])
				if (files[i[1]][0])
					*n += 1;
			strarrfree(files);
			break ;
		}
	}
}

static int	get_arg_num(char **args)
{
	int		i[5];
	int		n;

	i[2] = -1;
	n = 0;
	while (args[++i[2]])
		get_arg_num_loop(args, i, &n);
	return (n);
}

static void	wildcard_handler(char **args, char **newargs, int *i, int *i2)
{
	char	**files;
	char	**split;

	files = get_file_list(args[i[2]][0] == '.');
	i[1] = -1;
	split = ft_split(args[i[2]], '*');
	while (files[++i[1]])
		match(args[i[2]], split, files, i[1]);
	strarrfree(split);
	i[1] = -1;
	while (files[++i[1]])
	{
		if (files[i[1]][0])
		{
			newargs[*i2] = malloc(ft_strlen(files[i[1]]) + 3);
			ft_strlcpy(newargs[*i2], "\'", ft_strlen(files[i[1]]) + 3);
			ft_strlcat(newargs[*i2], files[i[1]], ft_strlen(files[i[1]]) + 3);
			ft_strlcat(newargs[*i2], "\'", ft_strlen(files[i[1]]) + 3);
			*i2 += 1;
		}
	}
	strarrfree(files);
}

char	**expand_wildcard(char **args)
{
	int		i[5];
	int		i2;
	int		n;
	char	**newargs;

	n = get_arg_num(args);
	newargs = malloc((strarrlen(args) + n + 1) * 8);
	i[2] = -1;
	i2 = 0;
	while (args[++i[2]])
	{
		i[0] = -1;
		while (args[i[2]][++i[0]])
		{
			quote_check(args[i[2]], i);
			if (!i[3] && !i[4] && args[i[2]][i[0]] == '*')
			{
				wildcard_handler(args, newargs, i, &i2);
				break ;
			}
		}
		if (!args[i[2]][i[0]])
			newargs[i2++] = ft_strdup(args[i[2]]);
	}
	strarrfree(args);
	newargs[i2] = 0;
	return (newargs);
}
