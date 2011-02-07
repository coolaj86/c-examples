#ifndef _PFD_H_
#define _PFD_H_

#define PFD_MAX_BUF 1920
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
};

// malloc and init the pfd
struct pfd*
pfd_create();

// free the pfd
void
pfd_destroy(struct pfd* pfd);

// Put a single byte in the socket
void 
pfd_make_readable(struct pfd* pfd);

// Read all bytes from the socket
void
pfd_make_unreadable(struct pfd* pfd);

// Take 1/2 of the packet size out of the socket
void
pfd_make_writable(struct pfd* pfd);

// Flood the buffer with data
void
pfd_make_unwritable(struct pfd* pfd);

#endif
