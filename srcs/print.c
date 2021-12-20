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

void format_print (char *type, char *desc, char *content, char *comment)
{
	char *c1;

	if (!cmp(type, "ETH"))
		c1 = RED;
	else
		c1 = YELLOW;

	printf("|  > (%s%s%s) %-30s %s%-20s%s%-20s\n", c1, type, RESET, desc, CYAN, content, RESET, comment);
}

void print_arp (char *type, struct ethhdr *eth, struct ether_arp *arp)
{
	char buffer[100];

	printf("=========================    %s    =========================\n|\n", type);

	printf("|  %sRaw packet%s\n|  > %s", GREEN, RESET, RED);
	for (int i = 0 ; i < 14 ; i++)
		printf("%02X ", ((unsigned char *)eth)[i]);
	for (int i = 0 ; i < 7 ; i++)
		printf("%02X ", ((unsigned char *)arp)[i]);
	printf("%s\n|  > %s", RESET, RED);
	for (int i = 7 ; i < 28 ; i++)
		printf("%02X ", ((unsigned char *)arp)[i]);
	printf("%s\n|\n", RESET);
	
	printf("|  %sProcessed%s\n", GREEN, RESET);
	sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
	format_print("ETH", "Target hardware address", buffer, "");

	sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
	format_print("ETH", "Source hardware address", buffer, "");
	
	sprintf(buffer, "x%04X", ntohs(eth->h_proto));
	format_print("ETH", "Protocol type", buffer, "(x0806 for ARP)");
	
	sprintf(buffer, "%u", ntohs(arp->ea_hdr.ar_hrd));
	format_print("ARP", "Hardware type", buffer, "(1 for Ethernet)");
	
	sprintf(buffer, "%u", ntohs(arp->ea_hdr.ar_pro));
	format_print("ARP", "Protocol type", buffer, "(2048 for IP)");
	
	sprintf(buffer, "%u", arp->ea_hdr.ar_hln);
	format_print("ARP", "Hardware address length", buffer, "(6 for MAC)");
	
	sprintf(buffer, "%u", arp->ea_hdr.ar_pln);
	format_print("ARP", "Protocol address length", buffer, "(4 for IPv4)");
	
	sprintf(buffer, "%u", ntohs(arp->ea_hdr.ar_op));
	format_print("ARP", "Operation", buffer, "(1 for request, 2 for reply)");
	
	sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", arp->arp_sha[0], arp->arp_sha[1], arp->arp_sha[2], arp->arp_sha[3], arp->arp_sha[4], arp->arp_sha[5]);
	format_print("ARP", "Source hardware address", buffer, "");
	
	sprintf(buffer, "%d.%d.%d.%d", arp->arp_spa[0], arp->arp_spa[1], arp->arp_spa[2], arp->arp_spa[3]);
	format_print("ARP", "Source protocol address", buffer, "");
	
	sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", arp->arp_tha[0], arp->arp_tha[1], arp->arp_tha[2], arp->arp_tha[3], arp->arp_tha[4], arp->arp_tha[5]);
	format_print("ARP", "Target hardware address", buffer, "");
	
	sprintf(buffer, "%d.%d.%d.%d", arp->arp_tpa[0], arp->arp_tpa[1], arp->arp_tpa[2], arp->arp_tpa[3]);
	format_print("ARP", "Target protocol address", buffer, "");
	
	printf("|\n\n");
}