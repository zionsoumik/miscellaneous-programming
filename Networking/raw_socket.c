#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>//AF_PACKET,SOCK_RAW DEFINED HERE
#include <linux/if_packet.h>//in cluded in arp
#include <linux/if_ether.h>//ethframelen,ethpall,included in arp
#include <linux/if_arp.h>//iffpromisc,
#include <string.h>
#include <sys/ioctl.h>//siocg...
#define pckt 5

char addrlist[120][17];
int num_addr=0;
int broad_adr=0;
int msgcount=0;
int main(void)
{
     int j=255;
     char add[17];
     sprintf(add, "%x:%x:%x:%x:%x:%x", j,j,j,j,j,j);

     int s,i; 

     s = socket(AF_PACKET,SOCK_RAW,ntohs(ETH_P_ALL));
     if (s == -1) { perror("Error: creating socket"); exit(0); }

     unsigned char* buffer =(unsigned char*)malloc(ETH_FRAME_LEN);

     int length = 0; 
     char addr[20];
     char dest[20];
     while(num_addr<=pckt)
     {
          memset(buffer, '\0', ETH_FRAME_LEN);

          length = recvfrom(s, buffer, ETH_FRAME_LEN, 0, (struct sockaddr *) 0,  (int *)0);
          if (length == -1)
          {
               perror("Error while receiving ethernet frame...\n");
               return 0;
          }

          else 
          {
	      msgcount++;
              sprintf(dest, "%x:%x:%x:%x:%x:%x", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
              int j;
              if(strcmp(dest, add)==0)
	      {
	          broad_adr++;
              }
                  sprintf(addr, "%x:%x:%x:%x:%x:%x", buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11]);
                  int found = 0;
                  for(i = 0 ; i < num_addr ; i++)
                  {
                       if(!strcmp(addrlist[i], addr))
	               {
	
                            found = 1;
		            break;
	               }
                  }
                  
                  if(!found)
                  {
                      if(num_addr==0)
                      printf("Station\t   Source_Address\t  Destination_addresss\n------------------------------------------------------\n");
                      strcpy(addrlist[num_addr], addr);
                      num_addr++;
                      printf("%d\t  %s\t  %s\n",num_addr,addr,dest);
                  }

             }

         
     }
         printf("after reciving %d ethernet packet percentage of ethernet packet is..\n",pckt);
         printf("total msg=%d\n",msgcount);
         float f;
         f=(float)broad_adr/msgcount;
         printf("broadcast msg=%d\n",broad_adr);
         printf("%f\n",f);
         close(s);
}
