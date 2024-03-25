/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 11:28:37 by dmonjas-          #+#    #+#             */
/*   Updated: 2024/03/25 19:55:17 by david            ###   ########.fr       */
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
			while (shell->env[i][j] != '=')
				j++;
			k = 0;
			while (shell->env[i][++j])
			{
				shell->env[i][j] = aux[k];
				k++;
			}
			break ;
		}
	}
	free(aux);
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
			while (shell->env[i][j] != '=')
				j++;
			k = 0;
			while (shell->env[i][++j])
			{
				shell->env[i][j] = aux[k];
				k++;
			}
			break ;
		}
	}
	free(aux);
}

static t_command	*ft_join(t_command *cmd, char *line)
{
	t_command	*pipe;
	t_command	*aux;

	pipe = NULL;
	aux = cmd;
	while (aux)
	{
		if (aux->command[0] == '|' && !aux->next)
			return (ft_er_out("`|'", 1), free(line), ft_free_cmd(&cmd), pipe);
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
	int		flag;

	line = NULL;
	flag = 0;
	if (shell->cmd_line[0] == '\0')
		return ;
	if (shell->cmd_line[0] == '<')
		flag = 1;
	signal(SIGINT, ft_intnl);
	signal(SIGQUIT, ft_quit);
	cmd = ft_take_cmd(&cmd, line, shell->cmd_line);
	if (!cmd)
		return ;
	ft_sust(&cmd, shell);
	ft_inout(&cmd, shell);
	cmd = ft_join(cmd, line);
	if (g_code_error != 0)
		return (ft_free_cmd(&cmd));
	ft_cmdtake(&cmd);
	if (flag)
		cmd->command = ft_swap(cmd->command, shell->inf);
	ft_system(cmd, shell, ft_check_in(shell), ft_check_out(shell));
	return (ft_free_cmd(&cmd));
}
