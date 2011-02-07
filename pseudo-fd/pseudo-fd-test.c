#include <pthread.h> // pthread_t, pthread_create
#include <string.h> // strerror
#include <stdio.h> // fprintf puts
#include <unistd.h> // sleep
#include <stdlib.h> // exit
#include <fcntl.h> // fcntl
#include <sys/socket.h> // setsockopt

#include "pseudo-fd.h"

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

  for (i = 0; i < 200000; i += 1)
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

  for (i = 0; i < 200000; i += 1)
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

  return 0;
}
