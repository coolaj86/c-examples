#include <pthread.h> // pthread_t, pthread_create
#include <string.h> // strerror
#include <stdio.h> // fprintf puts
#include <unistd.h> // sleep
#include <stdlib.h> // exit
#include <fcntl.h> // fcntl
#include <sys/socket.h> // setsockopt

#define PFD_MAX_BUF 8192
#define PFD_MIN_CHUNK 960

enum PFD_STATUS {
  PFD_NONE = 0,
  PFD_READ = 1,
  PFD_WRITE = 2,
  PFD_MAX = 3
};

struct pfd {
  // first two are ints
  // can be casted to int*
  int fd;
  int priv_fd;
  enum PFD_STATUS status;
  char buffer[PFD_MAX_BUF];
  int max_write;
};

/*
void* do_stuff_thread(void* args)
{
  struct fake_file ff;
  ff.fd = *((int*) args);

  while(1)
  {
    puts("I'm doing stuff");
    sleep(1);
  }
  return NULL;
}

void* do_things_thread(void* args)
{
  int fd = *((int*) args);

  while(1)
  {
    puts("I'm doing things");
    sleep(1);
  }
  return NULL;
}
*/

struct pfd*
pfd_create()
{
  int sv[2];
  struct pfd* pfd;
  int buflen = 1;
  int flags;
  int result = 0;

  // todo try SOCK_DGRAM
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
    perror("socketpair");
    exit(1);
  }

  // Put in non-block mode
  flags = fcntl(sv[0], F_GETFL);
  flags |= O_NONBLOCK;
  fcntl(sv[0], F_SETFL, flags);
  flags = fcntl(sv[1], F_GETFL);
  flags |= O_NONBLOCK;
  fcntl(sv[1], F_SETFL, flags);

  // Shrink buffer to two bytes (only works with AF_UNIX, not AF_INET)
  setsockopt(sv[0], SOL_SOCKET, SO_SNDBUF, &buflen, sizeof(int));
  setsockopt(sv[0], SOL_SOCKET, SO_RCVBUF, &buflen, sizeof(int));
  setsockopt(sv[1], SOL_SOCKET, SO_SNDBUF, &buflen, sizeof(int));
  setsockopt(sv[1], SOL_SOCKET, SO_RCVBUF, &buflen, sizeof(int));

  pfd = malloc(sizeof(struct pfd));
  pfd->fd = sv[0];
  pfd->priv_fd = sv[1];
  pfd->status = 0;
  pfd->max_write = 0;

  do 
  {
    result = write(pfd->fd, &pfd->buffer, PFD_MAX_BUF);
    pfd->max_write += result;
  } while (PFD_MAX_BUF == result);

  return pfd;
}

void
pfd_destroy(struct pfd* pfd)
{
  free(pfd);
}

void 
pfd_make_readable(struct pfd* pfd)
{
  int result;

  if (pfd->status & PFD_READ)
  {
    return;
  }
  pfd->status |= PFD_READ;

  result = write(pfd->priv_fd, &pfd->buffer, 1);
  if (1 == result)
  {
    return;
  }
  else
  {
    printf("should have written 1 byte but wrote %i\n", result);
  }
}

void
pfd_make_unreadable(struct pfd* pfd)
{
  int result;

  if (!(pfd->status & PFD_READ))
  {
    return;
  }
  pfd->status &= ~PFD_READ;

  result = read(pfd->fd, &pfd->buffer, PFD_MAX_BUF);
  if (1 != result)
  {
    printf("should have read %i byte but read %i\n", 1, result);
  }
}


// Take one byte off the buffer
void
pfd_make_writable(struct pfd* pfd)
{
  int result;

  if (pfd->status & PFD_WRITE)
  {
    return;
  }
  pfd->status |= PFD_WRITE;

  result = read(pfd->priv_fd, &pfd->buffer, PFD_MIN_CHUNK);
  if (PFD_MIN_CHUNK != result)
  {
    printf("should have read %i byte but read %i\n", PFD_MIN_CHUNK, result);
  }
}

// Put one byte back on the buffer
void
pfd_make_unwritable(struct pfd* pfd)
{
  int result;

  if (!(pfd->status & PFD_WRITE))
  {
    return;
  }
  pfd->status &= ~PFD_WRITE;

  result = write(pfd->fd, &pfd->buffer, PFD_MAX_BUF);
  if (PFD_MIN_CHUNK != result)
  {
    printf("should have written %i byte but wrote %i\n", PFD_MIN_CHUNK, result);
  }
}

void
assert(int maybe)
{
  static int count = 0;
  if (!maybe)
  {
    printf("%i\n", count);
    exit(1);
  }
  count += 1;
}

int
main(int argc, char* argv[])
{
  struct pfd* pfd;

  pfd = pfd_create();
  assert(!(PFD_READ & pfd->status));
  assert(!(PFD_WRITE & pfd->status));

  pfd_make_readable(pfd);
  assert(PFD_READ & pfd->status);
  pfd_make_readable(pfd);
  assert(PFD_READ & pfd->status);
  pfd_make_readable(pfd);
  assert(PFD_READ & pfd->status);

  pfd_make_unreadable(pfd);
  assert(!(PFD_READ & pfd->status));
  pfd_make_readable(pfd);
  assert(PFD_READ & pfd->status);

  pfd_make_unreadable(pfd);
  assert(!(PFD_READ & pfd->status));
  pfd_make_unreadable(pfd);
  assert(!(PFD_READ & pfd->status));
  pfd_make_unreadable(pfd);
  assert(!(PFD_READ & pfd->status));

  pfd_make_writable(pfd);
  assert(PFD_WRITE & pfd->status);
  pfd_make_writable(pfd);
  assert(PFD_WRITE & pfd->status);
  pfd_make_writable(pfd);
  assert(PFD_WRITE & pfd->status);

  pfd_make_unwritable(pfd);
  assert(!(PFD_WRITE & pfd->status));
  pfd_make_writable(pfd);
  assert(PFD_WRITE & pfd->status);

  pfd_make_unwritable(pfd);
  assert(!(PFD_WRITE & pfd->status));
  pfd_make_unwritable(pfd);
  assert(!(PFD_WRITE & pfd->status));
  pfd_make_unwritable(pfd);
  assert(!(PFD_WRITE & pfd->status));

  pfd_make_writable(pfd);
  assert(PFD_WRITE & pfd->status);
  /*
  */

  pfd_destroy(pfd);

  /*
  int thread_status;
  pthread_t do_stuff_pthread;
  pthread_t do_things_pthread;
  void* exit_status;

  int fd = pfd_create();


  puts("Hello World!");

  

  thread_status = pthread_create(&do_stuff_pthread, NULL, do_stuff_thread, sv[0]);
  if (0 != thread_status)
  {
    fprintf(stderr, "thread creation failed with errno %d (%s)\n", thread_status, strerror(thread_status));
  }

  thread_status = pthread_create(&do_things_pthread, NULL, do_things_thread, sv[1]);
  if (0 != thread_status)
  {
    fprintf(stderr, "thread creation failed with errno %d (%s)\n", thread_status, strerror(thread_status));
  }

  // If the threads are not joined, main will exit
  // and subsequently kill them, possibly never having been run
  pthread_join(do_stuff_pthread, &exit_status);
  pthread_join(do_things_pthread, &exit_status);
  
  puts("Good Bye!");
  */ 
  return 0;
}
