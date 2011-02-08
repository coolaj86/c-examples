#include <pthread.h> // pthread_t, pthread_create
#include <string.h> // strerror
#include <stdio.h> // fprintf puts
#include <unistd.h> // sleep
#include <stdlib.h> // exit
#include <fcntl.h> // fcntl
#include <sys/socket.h> // setsockopt

#include "pseudo-fd.h"

datagen_init

void* generate_data_thread(void* args)
{
  struct pfd* pfd;
  ff.fd = *((int*) args);

  while(1)
  {
    puts("I'm doing stuff");
    sleep(1);
  }
  return NULL;
}

void* consume_data_thread(void* args)
{
  int fd = *((int*) args);

  while(1)
  {
    puts("I'm doing things");
    sleep(1);
  }
  return NULL;
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
  int i;

  pfd = pfd_create();
  assert(!(PFD_READ & pfd->status));
  assert(!(PFD_WRITE & pfd->status));

  puts("\nREAD TESTS (passes perfectly if no output)");
  pfd_make_readable(pfd);
  assert(PFD_READ & pfd->status);
  pfd_make_readable(pfd);
  assert(PFD_READ & pfd->status);
  pfd_make_readable(pfd);
  assert(PFD_READ & pfd->status);

  for (i = 0; i < 2000000; i += 1)
  {
  pfd_make_unreadable(pfd);
  assert(!(PFD_READ & pfd->status));
  pfd_make_readable(pfd);
  assert(PFD_READ & pfd->status);
  }

  pfd_make_unreadable(pfd);
  assert(!(PFD_READ & pfd->status));
  pfd_make_unreadable(pfd);
  assert(!(PFD_READ & pfd->status));
  pfd_make_unreadable(pfd);
  assert(!(PFD_READ & pfd->status));

  puts("\n\nWRITE TESTS (passes if no output or read and write the same despite expectation)");
  pfd_make_writable(pfd);
  assert(PFD_WRITE & pfd->status);
  pfd_make_writable(pfd);
  assert(PFD_WRITE & pfd->status);
  pfd_make_writable(pfd);
  assert(PFD_WRITE & pfd->status);

  for (i = 0; i < 2000000; i += 1)
  {
  pfd_make_unwritable(pfd);
  assert(!(PFD_WRITE & pfd->status));
  pfd_make_writable(pfd);
  assert(PFD_WRITE & pfd->status);
  }

  pfd_make_unwritable(pfd);
  assert(!(PFD_WRITE & pfd->status));
  pfd_make_unwritable(pfd);
  assert(!(PFD_WRITE & pfd->status));
  pfd_make_unwritable(pfd);
  assert(!(PFD_WRITE & pfd->status));

  pfd_make_writable(pfd);
  assert(PFD_WRITE & pfd->status);

  pfd_destroy(pfd);

  /*
  int thread_status;
  pthread_t do_stuff_pthread;
  pthread_t do_things_pthread;
  void* exit_status;

  int fd = pfd_create();


  puts("Hello World!");

  

  thread_status = pthread_create(&do_stuff_pthread, NULL, generate_data_thread, sv[0]);
  if (0 != thread_status)
  {
    fprintf(stderr, "thread creation failed with errno %d (%s)\n", thread_status, strerror(thread_status));
  }

  thread_status = pthread_create(&do_things_pthread, NULL, consume_data_thread, sv[1]);
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
