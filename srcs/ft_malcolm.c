/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/01 18:43:59 by cclaude           #+#    #+#             */
/*   Updated: 2021/09/08 01:09:43 by cclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

int main (int ac, char **av)
{
	if (error_check(ac, av))
		return (1);

	int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	printf("Connected on socket : %d\n", packet_socket);

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
			if (ntohs(eth->h_proto) == ETH_P_ARP)
			{
				printf("Found ARP request ! (size %d)\n", packet_len);
				printf("|- Sender MAC : %02X:%02X:%02X:%02X:%02X:%02X\n", arp->arp_sha[0], arp->arp_sha[1], arp->arp_sha[2], arp->arp_sha[3], arp->arp_sha[4], arp->arp_sha[5]);
				printf("|- Sender IP : %d.%d.%d.%d\n", arp->arp_spa[0], arp->arp_spa[1], arp->arp_spa[2], arp->arp_spa[3]);
				printf("|- Target MAC : %02X:%02X:%02X:%02X:%02X:%02X\n", arp->arp_tha[0], arp->arp_tha[1], arp->arp_tha[2], arp->arp_tha[3], arp->arp_tha[4], arp->arp_tha[5]);
				printf("|- Target IP : %d.%d.%d.%d\n", arp->arp_tpa[0], arp->arp_tpa[1], arp->arp_tpa[2], arp->arp_tpa[3]);
				printf("> Full breakdown :\n");
				printf("> eth->h_dest %.6s\n", eth->h_dest);
				printf("> eth->h_source %.6s\n", eth->h_source);
				printf("> eth->h_proto %04X\n", eth->h_proto);
				printf("> arp->ea_hdr.ar_hrd %u\n", ntohs(arp->ea_hdr.ar_hrd));
				printf("> arp->ea_hdr.ar_pro %u\n", ntohs(arp->ea_hdr.ar_pro));
				printf("> arp->ea_hdr.ar_hln %u\n", arp->ea_hdr.ar_hln);
				printf("> arp->ea_hdr.ar_pln %u\n", arp->ea_hdr.ar_pln);
				printf("> arp->ea_hdr.ar_op %u\n", ntohs(arp->ea_hdr.ar_op));
				printf("> arp->arp_sha %.6s\n", arp->arp_sha);
				printf("> arp->arp_spa %u\n", *(unsigned int *)arp->arp_spa);
				printf("> arp->arp_tha %.6s\n", arp->arp_tha);
				printf("> arp->arp_tpa %u\n\n", *(unsigned int *)arp->arp_tpa);
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
