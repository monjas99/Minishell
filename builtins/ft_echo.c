/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodro <rodro@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 17:37:07 by dmonjas-          #+#    #+#             */
/*   Updated: 2024/01/31 16:19:11 by rodro            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* static int	ft_skip_n(char *cmd, int i)
{
	while (cmd[i])
	{
		if (cmd[i] == ' ')
			return (i);
		if (cmd[i] != 'n')
			return (0);
		i++;
	}
	return (0);
}

static int	ft_space_skip(char *cmd, int i)
{
	while (cmd[i] == ' ')
		i++;
	return (i);
}

static int	ft_check_nl(char *cmd)
{
	int	i;
	int	j;

	i = 4;
	j = 0;
	while (cmd[i] == ' ')
		i++;
	while (cmd[i] == '-')
	{
		j = i;
		i++;
		if (!ft_skip_n(cmd, i))
			return (j);
		else
			i = ft_skip_n(cmd, i);
		i = ft_space_skip(cmd, i);
	}
	return (i);
}

void	ft_echo(char *cmd, int fd)
{
	int	i;

	i = ft_check_nl(cmd);
	if (i > 5)
		ft_putstr_fd(&cmd[i], fd);
	else
	{
		ft_putstr_fd(&cmd[i], fd);
		ft_putstr_fd("\n", fd);
	}
} */

void	ft_echo(char *cmd, int fd)
{
	int	i;

	i = 5;
	if (!cmd[i - 1])
		ft_putstr_fd("\n", fd);
	if (cmd[i] == '-' && cmd[i + 1] == 'n')
		ft_putstr_fd(&cmd[i + 3], fd);
	else
	{
		ft_putstr_fd(&cmd[i], fd);
		ft_putstr_fd("\n", fd);
	}
}