#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

  fd_set readfds;
  int maxfd = 0;
  FD_ZERO(&readfds);
  for (int i = 0; i < NUM_PIPES; ++i) {
    FD_SET(pipes[i][0], &readfds);
    if (pipes[i][0] > maxfd) {
      maxfd = pipes[i][0];
    }
  }

  // 调用 select
  if (select(maxfd + 1, &readfds, NULL, NULL, NULL) == -1) {
    perror("select");
    exit(EXIT_FAILURE);
  }

  // 读取数据
  char buffer[1024];
  char result[1024 * NUM_PIPES];
  int result_len = 0;
  for (int i = 0; i < NUM_PIPES; ++i) {
    if (FD_ISSET(pipes[i][0], &readfds)) {
      ssize_t bytes_read = read(pipes[i][0], buffer, sizeof(buffer));
      if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        if (result_len > 0) {

          result[result_len++] = ',';
        }

        strcpy(result + result_len, buffer);
        result_len += bytes_read;
      }
    }
  }

  result[result_len] = '\0';
  printf("Merged data: %s\n", result);
}
