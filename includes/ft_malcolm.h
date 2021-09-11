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
# include <arpa/inet.h>
# include <netinet/if_ether.h>
# include <ifaddrs.h>
# include <netdb.h>
# include <netpacket/packet.h>

# define ETHER_ARP_LEN sizeof(struct ether_arp)

int error_check (int ac, char **av);

void fill_mac (unsigned char *addr, char *mac);
void memcopy (unsigned char *src, unsigned char *dst, size_t n);

void print_arp (struct ethhdr *eth, struct ether_arp *arp);
void print_addrll (struct sockaddr_ll *addr);

#endif
