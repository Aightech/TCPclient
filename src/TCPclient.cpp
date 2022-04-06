#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "TCPclient.h"



TCPclient::TCPclient()
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

TCPclient::~TCPclient(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}


int TCPclient::open_connection(const char *address, int port)
{
   m_sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };
   struct hostent *hostinfo;

   if(m_sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   hostinfo = gethostbyname(address);
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", address);
      exit(EXIT_FAILURE);
   }

   sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   sin.sin_port = htons(port);
   sin.sin_family = AF_INET;

   if(connect(m_sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
   {
      perror("connect()");
      exit(errno);
   }

   return m_sock;
}

int TCPclient::close_connection()
{
   return closesocket(m_sock);
}

int TCPclient::read(uint8_t *buffer)
{
   int n = 0;

   if((n = recv(m_sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      exit(errno);
   }
   buffer[n] = 0;
   return n;
}

int TCPclient::write(const void *buffer, size_t size)
{
  int n=0;
  if((n=send(m_sock, buffer, size, 0)) < 0)
   {
      perror("send()");
      exit(errno);
   }
  return n;
}

