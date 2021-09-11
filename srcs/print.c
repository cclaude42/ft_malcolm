/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 19:26:05 by cclaude           #+#    #+#             */
/*   Updated: 2021/09/11 20:31:44 by cclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void print_arp (struct ethhdr *eth, struct ether_arp *arp)
{
	printf(" > eth->h_dest          %02X:%02X:%02X:%02X:%02X:%02X\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
	printf(" > eth->h_source        %02X:%02X:%02X:%02X:%02X:%02X\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
	printf(" > eth->h_proto         %04X                 (0806 for ARP)\n", ntohs(eth->h_proto));
	printf(" =======================\n");
	printf(" > arp->ea_hdr.ar_hrd   %-20u (1 for Ethernet)\n", ntohs(arp->ea_hdr.ar_hrd));
	printf(" > arp->ea_hdr.ar_pro   %-20u (2048 for IP)\n", ntohs(arp->ea_hdr.ar_pro));
	printf(" > arp->ea_hdr.ar_hln   %-20u (6 for MAC)\n", arp->ea_hdr.ar_hln);
	printf(" > arp->ea_hdr.ar_pln   %-20u (4 for IPv4)\n", arp->ea_hdr.ar_pln);
	printf(" > arp->ea_hdr.ar_op    %-20u (1 for request, 2 for reply)\n", ntohs(arp->ea_hdr.ar_op));
	printf(" > arp->arp_sha         %02X:%02X:%02X:%02X:%02X:%02X\n", arp->arp_sha[0], arp->arp_sha[1], arp->arp_sha[2], arp->arp_sha[3], arp->arp_sha[4], arp->arp_sha[5]);
	printf(" > arp->arp_spa         %d.%d.%d.%d\n", arp->arp_spa[0], arp->arp_spa[1], arp->arp_spa[2], arp->arp_spa[3]);
	printf(" > arp->arp_tha         %02X:%02X:%02X:%02X:%02X:%02X\n", arp->arp_tha[0], arp->arp_tha[1], arp->arp_tha[2], arp->arp_tha[3], arp->arp_tha[4], arp->arp_tha[5]);
	printf(" > arp->arp_tpa         %d.%d.%d.%d\n", arp->arp_tpa[0], arp->arp_tpa[1], arp->arp_tpa[2], arp->arp_tpa[3]);
	printf("\n");
}

void print_addrll (struct sockaddr_ll *addr)
{
	printf(" > addr->sll_family     %hu\n", addr->sll_family);
	printf(" > addr->sll_protocol   %hu\n", addr->sll_protocol);
	printf(" > addr->sll_ifindex    %d\n", addr->sll_ifindex);
	printf(" > addr->sll_hatype     %hu\n", addr->sll_hatype);
	printf(" > addr->sll_pkttype    %d\n", addr->sll_pkttype);
	printf(" > addr->sll_halen      %d\n", addr->sll_halen);
	printf(" > addr->sll_addr       %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", addr->sll_addr[0], addr->sll_addr[1], addr->sll_addr[2], addr->sll_addr[3], addr->sll_addr[4], addr->sll_addr[5], addr->sll_addr[6], addr->sll_addr[7]);
	printf("\n");
}
