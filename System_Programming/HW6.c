#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1

int main() {
  pid_t pid, pid2, pid3;
  int fd[2], fd2[2];

  pipe(fd);


  if((pid = fork()) == 0) {
    // child
    close(STDIN_FILENO);
    dup(fd[READ_END]);
    close(fd[WRITE_END]);
    close(fd[READ_END]);

    pipe(fd2);

    if ((pid2 = fork()) == 0) {
      // child

      if((pid3 = fork()) == 0) {
        close(STDOUT_FILENO);
        int ffd = open("./the_result", O_CREAT | O_RDWR , 0666);
        dup(ffd);
        // close(ffd);
        close(STDIN_FILENO);
        dup(fd2[READ_END]);
        close(fd2[WRITE_END]);
        close(fd2[READ_END]);
        execlp("wc", "wc", NULL);
      } else {
        close(fd2[READ_END]);
        close(fd2[WRITE_END]);
        waitpid(pid3, NULL, 0);
      }
    } else {
      // parent

      close(STDOUT_FILENO);
      dup(fd2[WRITE_END]);
      close(fd2[READ_END]);
      execlp("grep", "grep", "40947047s", NULL);
    }

  } else {
    // parent

    close(STDOUT_FILENO);
    dup(fd[WRITE_END]);
    close(fd[READ_END]);
    close(fd[WRITE_END]);
    execlp("ps", "ps", "aux", NULL);
  }

}
