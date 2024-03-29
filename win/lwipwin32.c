// lwipwin32.cpp : Defines the entry point for the console application.
//
#include <stdio.h>

#include "lwip/netif.h" 
#include "lwip/tcpip.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "net_tap.h"

extern err_t ethernetif_init(struct netif *netif);
extern void http_init();
extern void http_task();

void init()
{
  struct ip_addr ip, mask, gw;
  static struct netif netif;

  // see SYS_ARCH_PROTECT
  InitializeCriticalSection(&gCriticalSection);

  tcpip_init(NULL, NULL);

  // simple server
  sys_thread_new("http thread", http_task, NULL, 0, 0);
  //http_init();

  IP4_ADDR(&ip, 192, 168, 10, 161);
  IP4_ADDR(&mask, 255, 255, 255, 0);
  IP4_ADDR(&gw, 192, 168, 10, 1);

  netif_add(&netif, &ip, &mask, &gw, NULL, ethernetif_init, tcpip_input);
  netif_set_default(&netif);
  netif_set_up(&netif);
}

int main(int argc, char* argv[])
{
  if (ERR_IF == open_tap())
    exit(1);

  init();

  while(TRUE)
    Sleep(1000);

  // Todo close_dev();

  return 0;
}

