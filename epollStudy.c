#include <sys/epoll.h>

int main() {
  int epfd = epoll_create1(0);
  if (epfd == -1) {
    // deal error
  }

  struct epoll_event event;
  event.data.fd = sockfd; // sockfd 是你要监视的文件描述符
  event.events = EPOLLIN; // 指定你感兴趣的事件，EPOLLIN 表示可读事件

  if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event) == -1) {
    // 处理错误
  }

  const int MAX_EVENTS = 10;
  struct epoll_event events[MAX_EVENTS];
  int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
  if (nfds == -1) {
    // 处理错误
  }

  for (int n = 0; n < nfds; ++n) {
    if (events[n].events & EPOLLIN) {
        // 文件描述符 events[n].data.fd 可读
        // 进行读取操作
    }
    // 处理其他事件类型...
}
}