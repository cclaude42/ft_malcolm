/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/01 18:43:59 by cclaude           #+#    #+#             */
/*   Updated: 2021/09/07 20:20:53 by cclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

int main (int ac, char **av)
{
	if (error_check(ac, av))
		return (1);

	int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	printf("Connected on socket : %d\n", packet_socket);

	char buf[1000] = {0};
	// struct sockaddr_in saddr;
	// socklen_t addrlen;

	// saddr.sin_addr.s_addr = 0;
	// while (saddr.sin_addr.s_addr != inet_addr(av[3]))
	while (1)
	{
		// printf("IP found isn't target (%s), still waiting\n\n", av[3]);
		// int ret = recvfrom(packet_socket, buf, 1000, 0, (struct sockaddr *)&saddr, &addrlen);
		int ret = recvfrom(packet_socket, buf, 1000, 0, NULL, NULL);

		if (ret >= (int)sizeof(struct ethhdr))
		{
			struct ethhdr *eth = (struct ethhdr *)buf;
			if (ntohs(eth->h_proto) == ETH_P_ARP)
			{
				struct ether_arp *arp = (struct ether_arp *)(buf + ETHER_HDR_LEN);

				printf("Found ARP package !\n");
				printf("|- Sender MAC : %02X:%02X:%02X:%02X:%02X:%02X\n", arp->arp_sha[0], arp->arp_sha[1], arp->arp_sha[2], arp->arp_sha[3], arp->arp_sha[4], arp->arp_sha[5]);
				printf("|- Sender IP : %d.%d.%d.%d\n", arp->arp_spa[0], arp->arp_spa[1], arp->arp_spa[2], arp->arp_spa[3]);
				printf("|- Target MAC : %02X:%02X:%02X:%02X:%02X:%02X\n", arp->arp_tha[0], arp->arp_tha[1], arp->arp_tha[2], arp->arp_tha[3], arp->arp_tha[4], arp->arp_tha[5]);
				printf("|- Target IP : %d.%d.%d.%d\n\n", arp->arp_tpa[0], arp->arp_tpa[1], arp->arp_tpa[2], arp->arp_tpa[3]);
			}
		}

		// printf("Received : [%s] (ret %d)\n", buf, ret);
		// printf("Sender's IP : %s\n", inet_ntoa(saddr.sin_addr));
	}

	close(packet_socket);

	return (0);
}
