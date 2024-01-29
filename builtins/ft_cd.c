/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 17:37:03 by dmonjas-          #+#    #+#             */
/*   Updated: 2024/01/25 16:07:52 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_update(t_minishell *shell)
{
	int		i;
	char	*pwd;
	char	*oldpwd;

	pwd = ft_strjoin("PWD=", shell->pwd);
	oldpwd = ft_strjoin("OLDPWD=", shell->oldpwd);
	i = -1;
	while (shell->env[++i])
	{
		if (!ft_strncmp(shell->env[i], "PWD=", 4))
		{
			free(shell->env[i]);
			shell->env[i] = ft_strdup(pwd);
		}
		if (!ft_strncmp(shell->env[i], "OLDPWD=", 7))
		{
			free(shell->env[i]);
			shell->env[i] = ft_strdup(oldpwd);
		}
	}
	free(pwd);
	free(oldpwd);
}

static void	ft_new_pwd(t_minishell *shell, char *oldpwd, char *new_pwd)
{
	free(shell->pwd);
	free(shell->oldpwd);
	shell->pwd = ft_strdup(new_pwd);
	shell->oldpwd = ft_strdup(oldpwd);
	ft_update(shell);
}

void	ft_cd(char *cmd, t_minishell *shell)
{
	char	new_pwd[1024];
	char	*oldpwd;
	char	**comm;

	comm = ft_split(cmd, ' ');
	if (comm[1] && comm[1][ft_strlen(comm[1]) - 1] == '/')
		comm[1][ft_strlen(comm[1]) - 1] = '\0';
	if (!comm[1] || (comm[1][0] == '~' && ft_strlen(comm[1]) == 1))
	{
		if (chdir(shell->root) != 0)
			ft_per(comm[0], comm[1]);
	}
	else if (comm[1][0] == '-')
	{
		if (chdir(shell->oldpwd) != 0)
			ft_per(comm[0], comm[1]);
	}
	else if (chdir(comm[1]) != 0)
		ft_per(comm[0], comm[1]);
	oldpwd = ft_strdup(shell->pwd);
	if (getcwd(new_pwd, sizeof(new_pwd) - 1))
		ft_new_pwd(shell, oldpwd, new_pwd);
}
