/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 */

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>

//#define _POSIX_C_SOURCE 200112L
#include <time.h>
#include <errno.h>

#define MY_DEST_MAC0    0x00
#define MY_DEST_MAC1    0x00
#define MY_DEST_MAC2    0x00
#define MY_DEST_MAC3    0x00
#define MY_DEST_MAC4    0x00
#define MY_DEST_MAC5    0x00

#define DEFAULT_IF  "vlc0"
#define BUF_SIZ     16

/* Sleep; returns un-slept (leftover) time.
 */
double dsleep(const double seconds)
{
    const long sec = (long)seconds;
    const long nsec = (long)( (seconds - (double)sec) * 1e9 );
    struct timespec req, rem;

    if (sec < 0L)
    {
        return 0.0;
    }

    if (sec == 0L && nsec <= 0L)
    {
        return 0.0;
    }

    req.tv_sec = sec;
    if (nsec <= 0L)
    {
        req.tv_nsec = 0L;
    }
    else if (nsec <= 999999999L)
    {
        req.tv_nsec = nsec;
    }
    else
    {
        req.tv_nsec = 999999999L;
    }

    rem.tv_sec = 0;
    rem.tv_nsec = 0;

    if (nanosleep(&req, &rem) == -1)
    {
        if (errno == EINTR)
        {
            return (double)rem.tv_sec + (double)rem.tv_nsec / 1000000000.0;
        }
        else
        {
            return seconds;
        }
    }
    else
    {
        return 0.0;
    }
}


int main(int argc, char *argv[])
{
    int i,j,k;

    //char c,b;
    //c=getchar();
    //b=getchar();
    //getchar();

    int sockfd;
    struct ifreq if_idx;
    struct ifreq if_mac;
    int tx_len = 0;
    char sendbuf[BUF_SIZ];
    struct ether_header *eh = (struct ether_header *)sendbuf;
    struct iphdr *iph = (struct iphdr *)( sendbuf + sizeof(struct ether_header) );
    struct sockaddr_ll socket_address;
    char ifName[IFNAMSIZ];

    /* Get interface name */
    if (argc > 1)
    {
        strcpy(ifName, argv[1]);
    }
    else
    {
        strcpy(ifName, DEFAULT_IF);
    }

    /* Open RAW socket to send on */
    if ( ( sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW) ) == -1 )
    {
        perror("socket");
    }

    /* Get the index of the interface to send on */
    memset( &if_idx, 0, sizeof(struct ifreq) );
    strncpy(if_idx.ifr_name, ifName, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
    {
        perror("SIOCGIFINDEX");
    }

    /* Get the MAC address of the interface to send on */
    memset( &if_mac, 0, sizeof(struct ifreq) );
    strncpy(if_mac.ifr_name, ifName, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
    {
        perror("SIOCGIFHWADDR");
    }

    // Construct the Ethernet header
    memset(sendbuf, 0, BUF_SIZ);
    /*
       // Ethernet header
       eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
       eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
       eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
       eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
       eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
       eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
       eh->ether_dhost[0] = MY_DEST_MAC0;
       eh->ether_dhost[1] = MY_DEST_MAC1;
       eh->ether_dhost[2] = MY_DEST_MAC2;
       eh->ether_dhost[3] = MY_DEST_MAC3;
       eh->ether_dhost[4] = MY_DEST_MAC4;
       eh->ether_dhost[5] = MY_DEST_MAC5;
       // Ethertype field
       eh->ether_type = htons(ETH_P_IP);
       tx_len += sizeof(struct ether_header);*/

    /* Packet data */

    sendbuf[tx_len++] = 0xde; //sendbuf[tx_len++] = 0xde;

    sendbuf[tx_len++] = 0xaf;
    sendbuf[tx_len++] = 0xbe;
    /*
       sendbuf[tx_len++] = 0xef;
     */

    /* Index of the network device */
    socket_address.sll_ifindex = if_idx.ifr_ifindex;
    /* Address length*/
    socket_address.sll_halen =
        ETH_ALEN;
    /* Destination MAC */
    socket_address.sll_addr[0] = MY_DEST_MAC0;
    socket_address.sll_addr[1] = MY_DEST_MAC1;
    socket_address.sll_addr[2] = MY_DEST_MAC2;
    socket_address.sll_addr[3] = MY_DEST_MAC3;
    socket_address.sll_addr[4] = MY_DEST_MAC4;
    socket_address.sll_addr[5] = MY_DEST_MAC5;

    /* Send packet */
    for (j = 0; j < 5; j++)
    {
        for (i = 0; i < 10; i++)
        {
            if (sendto( sockfd, sendbuf, tx_len, 0, (struct sockaddr *)&socket_address, sizeof(struct sockaddr_ll) ) < 0)
            {
                printf("Send failed\n");
            }
            else
            {
                //printf("Sent\n");
                // sleepms(1000);
            }
        }

        //sleep(3.5);
        dsleep(3.9);
        //shutdown (sockfd, 2);
    }

    return 0;
}
