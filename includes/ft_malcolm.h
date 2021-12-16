/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/01 19:49:40 by cclaude           #+#    #+#             */
/*   Updated: 2021/09/11 20:28:06 by cclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <arpa/inet.h>
# include <netinet/if_ether.h>
# include <ifaddrs.h>
# include <netdb.h>
# include <netpacket/packet.h>

# define ETHER_ARP_LEN sizeof(struct ether_arp)

# define BLACK "\033[0;30m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"
# define RESET "\033[0m"

int error_check (int ac, char **av);

void fill_mac (unsigned char *addr, char *mac);
void memcopy (unsigned char *src, unsigned char *dst, size_t n);
int	cmp(const char *s1, const char *s2);

void print_arp (char *type, struct ethhdr *eth, struct ether_arp *arp);
void print_addrll (struct sockaddr_ll *addr);

#endif
