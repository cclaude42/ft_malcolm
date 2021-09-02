/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/01 18:43:59 by cclaude           #+#    #+#             */
/*   Updated: 2021/09/02 00:34:25 by cclaude          ###   ########.fr       */
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
	struct sockaddr_in saddr;
	socklen_t addrlen;

	saddr.sin_addr.s_addr = 0;
	while (saddr.sin_addr.s_addr != inet_addr(av[3]))
	{
		printf("IP found isn't target (%s), still waiting\n\n", av[3]);
		int ret = recvfrom(packet_socket, buf, 1000, 0, (struct sockaddr *)&saddr, &addrlen);

		printf("Received : [%s] (ret %d)\n", buf, ret);
		printf("Sender's IP : %s\n", inet_ntoa(saddr.sin_addr));
	}

	close(packet_socket);

	return (0);
}
