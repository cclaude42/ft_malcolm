/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/01 19:49:40 by cclaude           #+#    #+#             */
/*   Updated: 2021/09/08 03:31:08 by cclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

# include <unistd.h>
# include <stdio.h>
# include <arpa/inet.h>
# include <netinet/if_ether.h>
# include <ifaddrs.h>

# define ETHER_ARP_LEN sizeof(struct ether_arp)

int error_check (int ac, char **av);

#endif
