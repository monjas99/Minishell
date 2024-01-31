/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonjas- <dmonjas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 11:28:37 by dmonjas-          #+#    #+#             */
/*   Updated: 2024/01/31 11:44:05 by dmonjas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void ft_p_list(t_command *cmd)
{
	int i = 1;

	while (cmd)
	{
		ft_printf("Lista numero: %d\n", i);
		ft_printf("%s\n", cmd->command);
		if (cmd->infile != NULL)
			ft_printf("infile: %s\n", cmd->infile);
		if (cmd->outfile != NULL)
			ft_printf("outfile: %s\n", cmd->outfile);
		ft_printf("%s\n", cmd->built);
		ft_printf("comillas simples: %d\n", cmd->dollar);
		cmd = cmd->next;
		i++;
	}
}

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

static t_command	*ft_join(t_command **cmd)
{
	t_command	**pipe;
	t_command	*aux;
	char		*line;

	pipe = malloc(sizeof(t_command));
	*pipe = 0;
	line = NULL;
	aux = *cmd;
	while (aux)
	{
		if (ft_strlen(aux->command) == 1 && aux->command[0] == '|')
		{
			ft_lstadd_back_shell(pipe, ft_lstnew_shell(line));
			free(line);
			line = NULL;
			aux = aux->next;
		}
		line = ft_strjoin_gnl(line, aux->command);
		if (aux->next && aux->space == 0 && aux->next->command[0] != '|')
			line = ft_strjoin_gnl(line, " ");
		aux = aux->next;
	}
	ft_lstadd_back_shell(pipe, ft_lstnew_shell(line));
	//ft_free_cmd(cmd);
	return (free(line), *pipe);
}

void	ft_check_line(t_command *cmd, t_minishell *shell)
{
	char	*line;
	char	*cmd_line;
	int		flag;

	line = NULL;
	flag = 0;
	cmd_line = shell->cmd_line;
	if (cmd_line[0] == '\0')
		return ;
	if (cmd_line[0] == '<')
		flag = 1;
	signal(SIGQUIT, ft_quit);
	cmd = ft_take_cmd(&cmd, line, cmd_line);
	//ft_p_list(cmd);
	cmd = ft_sust(&cmd, shell->env);
	ft_inout(&cmd, shell);
	cmd = ft_join(&cmd);
	ft_cmdtake(&cmd);
	if (g_code_error != 0)
		return ;
	if (flag)
		ft_swap(cmd);
	ft_system(cmd, shell, ft_check_in(shell), ft_check_out(shell));
}

char	*ft_spr(char *line)
{
	int		i;
	int		j;
	int		k;
	char	*word;

	i = 0;
	j = 0;
	k = 0;
	while (line[i] && line[i] != ' ')
		i--;
	while (line[j] && line[j] != ' ')
		j++;
	i++;
	word = malloc(sizeof(char) * j + (i * -1));
	while (i < j)
	{
		word[k] = line[i];
		i++;
		k++;
	}
	word[k] = '\0';
	return (word);
}
