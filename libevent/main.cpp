#include <arpa/inet.h>
#include <errno.h>
#include <event2/event.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>

void read_callback(evutil_socket_t fd, short what, void *arg) {
  std::cout << "read\n";
}
void write_callback(evutil_socket_t fd, short what, void *arg) {
  std::cout << "write\n";
}

int main() {
  uint32_t ip = inet_addr("172.30.1.210");
  uint16_t port = 13444;
  int32_t sock = socket(PF_INET, SOCK_STREAM, 0);
  int32_t evfd;
  struct event *e_r, *e_w;
  struct event_base *base = event_base_new();
  uint32_t status = 0;

  if (sock < 0) {
    std::cout << "failed create socket"
              << "\n";
  }

  int on = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  struct sockaddr_in sock_addr;
  memset(&sock_addr, 0x00, sizeof(sock_addr));
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = ip;
  sock_addr.sin_port = htons(port);

  if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) {
    return -1;
  }

  if (connect(sock, (const sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
    std::cout << "failed connect - " << strerror(errno) << "\n";
  }

  e_r = event_new(base, sock, EV_READ | EV_PERSIST, read_callback, NULL);
  e_w = event_new(base, sock, EV_WRITE, write_callback, NULL);

  event_add(e_r, NULL);
  event_add(e_w, NULL);

  event_base_dispatch(base);

  return 0;
}