/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 11:28:37 by dmonjas-          #+#    #+#             */
/*   Updated: 2024/03/19 22:37:51 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_shell_up(t_minishell *shell)
{
	int		i;
	int		j;
	int		k;
	char	*aux;

	shell->shlvl += 1;
	i = -1;
	aux = ft_itoa(shell->shlvl);
	while (shell->env[++i])
	{
		if (!ft_strncmp(shell->env[i], "SHLVL", 5))
		{
			j = -1;
			while (shell->env[i][++j])
				if (shell->env[i][j] == '=')
					break ;
			k = 0;
			while (shell->env[i][++j])
			{
				shell->env[i][j] = aux[k];
				k++;
			}
			break ;
		}
	}
}

void	ft_shell_down(t_minishell *shell)
{
	int		i;
	int		j;
	int		k;
	char	*aux;

	shell->shlvl -= 1;
	i = -1;
	aux = ft_itoa(shell->shlvl);
	while (shell->env[++i])
	{
		if (!ft_strncmp(shell->env[i], "SHLVL", 5))
		{
			j = -1;
			while (shell->env[i][++j])
				if (shell->env[i][j] == '=')
					break ;
			k = 0;
			while (shell->env[i][++j])
			{
				shell->env[i][j] = aux[k];
				k++;
			}
			break ;
		}
	}
}

static t_command	*ft_join(t_command *cmd)
{
	t_command	*pipe;
	t_command	*aux;
	char		*line;

	pipe = NULL;
	line = NULL;
	aux = cmd;
	while (aux)
	{
		if (ft_strlen(aux->command) == 1 && aux->command[0] == '|')
		{
			ft_lstadd_back_shell(&pipe, ft_lstnew_shell(line));
			free(line);
			line = NULL;
			aux = aux->next;
		}
		line = ft_strjoin_gnl(line, aux->command);
		if (aux->next && aux->next->command
			&& aux->space == 0 && aux->next->command[0] != '|')
			line = ft_strjoin_gnl(line, " ");
		aux = aux->next;
	}
	ft_lstadd_back_shell(&pipe, ft_lstnew_shell(line));
	ft_free_cmd(&cmd);
	return (free(line), pipe);
}

void	ft_check_line(t_command *cmd, t_minishell *shell)
{
	char	*line;
	char	*cmd_line;
	int		flag;

	line = NULL;
	cmd_line = shell->cmd_line;
	if (cmd_line[0] == '\0')
		return ;
	flag = ft_flag_parse(cmd_line);
	signal(SIGINT, ft_intnl);
	signal(SIGQUIT, ft_quit);
	cmd = ft_take_cmd(&cmd, line, cmd_line);
	if (!cmd)
		return ;
	ft_sust(&cmd, shell);
	ft_inout(&cmd, shell);
	cmd = ft_join(cmd);
	ft_cmdtake(&cmd);
	if (g_code_error != 0)
		return (ft_free_cmd(&cmd));
	if (flag)
		cmd->command = ft_swap(cmd->command, shell->inf);
	return (ft_system(cmd, shell, ft_check_in(shell),
			ft_check_out(shell)), ft_free_cmd(&cmd));
}
