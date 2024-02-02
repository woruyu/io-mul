#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#define NUM_PIPES 5

int pipes[NUM_PIPES][2];
const char *message[NUM_PIPES] = {"cat", "dog", "monkey", "lion", "tiger"};

void *write_to_pipes(void *arg) {
  sleep(1);
  for (int i = 0; i < NUM_PIPES; ++i) {
    write(pipes[i][1], message[i], strlen(message[i]));
    write(pipes[i][1], message[i], strlen(message[i]));
    write(pipes[i][1], message[i], strlen(message[i]));
    write(pipes[i][1], message[i], strlen(message[i]));
    write(pipes[i][1], message[i], strlen(message[i]));
  }
  return NULL;
}

int main() {
  pthread_t writer_thread;

  // 创建管道
  for (int i = 0; i < NUM_PIPES; ++i) {
    if (pipe(pipes[i]) == -1) {
      perror("pipe");
      exit(EXIT_FAILURE);
    }
  }

  pthread_create(&writer_thread, NULL, write_to_pipes, NULL);

  int epfd = epoll_create1(0);
  if (epfd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  struct epoll_event ev, events[NUM_PIPES];
  for (int i = 0; i < NUM_PIPES; ++i) {
    ev.events = EPOLLIN;
    ev.data.fd = pipes[i][0];
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, pipes[i][0], &ev) == -1) {
      perror("epoll_ctl");
      exit(EXIT_FAILURE);
    }
  }

  // 调用 epoll_wait
  int nfds = epoll_wait(epfd, events, NUM_PIPES, -1);
  if (nfds == -1) {
    perror("epoll_wait");
    exit(EXIT_FAILURE);
  }

  // 读取数据
  char buffer[1024];
  char result[1024 * NUM_PIPES];
  int result_len = 0;
  for (int i = 0; i < nfds; ++i) {
    ssize_t bytes_read = read(events[i].data.fd, buffer, sizeof(buffer));
    if (bytes_read > 0) {
      buffer[bytes_read] = '\0';
      if (result_len > 0) {

        result[result_len++] = ',';
      }

      strcpy(result + result_len, buffer);
      result_len += bytes_read;
    }
  }

  result[result_len] = '\0';
  printf("Merged data: %s\n", result);
}
