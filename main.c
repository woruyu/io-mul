#include <poll.h>

struct pollfd {
  int fd;            /* File descriptor to poll.  */
  short int events;  /* Types of events poller cares about.  */
  short int revents; /* Types of events that actually occurred.  */
};

int poll(struct pollfd *fds, nfds_t nfds, int timeout);

#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

#include <sys/epoll.h>

int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);