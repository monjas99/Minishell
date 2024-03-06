/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 10:48:52 by dmonjas-          #+#    #+#             */
/*   Updated: 2024/03/05 20:20:02 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*ft_comp_list(t_command	*cmd)
{
	if (ft_lst_size(cmd) == 1 || ft_lst_size(cmd) == 2)
	{
		if (ft_strchr(cmd->command, '<'))
			return (ft_printf("syntax error near unexpected token `newline'\n") ,NULL);
		else if (ft_strchr(cmd->command, '>') || cmd->command[1] == '>')
			return (ft_printf("syntax error near unexpected token `newline'\n"), NULL);
		else if (ft_strchr(cmd->command, '|'))
			return (ft_printf("syntax error near unexpected token `|'\n"), NULL);
		else if(cmd->command[0] == '<' && cmd->command[1] == '<' && cmd->next->command[0] == '|')
			return (ft_printf("syntax error near unexpected token `newline'\n"), NULL);
	}
	else
	{
		if(cmd->command[0] == '<' && cmd->command[1] == '<' && cmd->next->command[0] == '|')
			return (ft_printf("syntax error near unexpected token `|'\n"), NULL);
	}
	return (cmd);
}

static void	ft_why(t_command *cmd, t_command **change)
{
	t_command	*aux;
	t_command	*aux_change;

	aux = cmd;
	aux_change = *change;
	*change = aux_change->next;
	while (cmd && (ft_strnstr(aux->next->command, "$", ft_strlen(aux->next->command)) == 0))
		aux = aux->next;
	aux->next = aux->next->next;
}

static char	*ft_change_doll(char *fir_line, char *sec_line, t_minishell *shell)
{
	char	*line;

	line = ft_strjoin_gnl(fir_line, ft_itoa(shell->last_error));
	if (sec_line)
		line = ft_strjoin_gnl(line, sec_line);
	return (line);
}

char	*ft_sust_doll(char *line, t_minishell *shell)
{
	int		i;
	int		j;
	char	*fir_line;
	char	*sec_line;

	i = 0;
	fir_line = NULL;
	sec_line = NULL;
	while (line[i] != '$' && line[i])
		i++;
	fir_line = ft_substr(line, 0, i);
	j = i;
	j += 2;
	if (line[j])
		sec_line = ft_substr(line, j, ft_strlen(&line[j]));
	line = ft_change_doll(fir_line, sec_line, shell);
	return (line);
}

t_command	*ft_select_sust(t_command **cmd, t_command *aux, t_minishell *shell)
{

	if (ft_strnstr(aux->command, "$", ft_strlen(aux->command)) != 0
			 && ft_strlen(aux->command) == 1 
			 && (aux->space == 0 || (aux->space == 1 && aux->next == NULL)))
			aux = aux->next;
	else if (ft_strnstr(aux->command, "$?", ft_strlen(aux->command)) != 0)
			aux->command = ft_sust_doll(aux->command, shell);
	else if (ft_strnstr(aux->command, "$", ft_strlen(aux->command)) != 0
			&& ft_strlen(aux->command) == 1 && aux->space == 1)
			ft_why(*cmd, &aux);
	else if (ft_strnstr(aux->command, "$", ft_strlen(aux->command)) != 0)
			aux->command = ft_param(aux->command, shell->env);
	else if (ft_strnstr(aux->command, "$", ft_strlen(aux->command)) == 0)
			aux = aux->next;
	return (aux);
}
