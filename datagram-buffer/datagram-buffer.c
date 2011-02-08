struct dg_buffer {
  int fd;
  size_t size;
  int count;
  char read_lock;
  void* start;
  void* read_index;
  void* write_index;
  struct pfd; pfd;
};

// Size of the buffer and the number of buffers
struct dg_buffer* dgctl_create(size_t size, int count);

void* dg_provider_thread(void* args);

// Lock the buffer
int dgctl_read_lock(struct dg_buffer* buf);

// Release the buffer
int dgctl_release(struct dg_buffer* buf);

int dgctl_write(struct dg_buffer* buf);

int dgctl_destroy(struct dg_buffer* buf);



struct dg_buffer* dgctl_create(struct dg_buffer* dg_buf, size_t size, int count)
{
  dg_buf->start = malloc(size * count);
  dg_buf->size = size;
  dg_buf->count = count;
  dg_buf->read_index = NULL;
  dg_buf->write_index = dg_buf->start;
}

void dgctl_get_writable(dg_buf, data)
{
  
}

void* dg_provider_thread(void* args)
{
  static char datum;
  static char prime = 7;
  static int i;
  struct dg_buffer* dg_buf = (struct dg_buffer*) args;
  char* data;
  char* datump;

  while (1)
  {
    // TODO use callback
    dgctl_lock_write(dg_buf);
    data = dg_buf();
    datump = data;
    usleep(100 * 1000);
    // emulate reading from a device buffer such as fread();
    for (i = 0; i < dg_buf->size; i += 1)
    {
      *datump = datum;
      datump += 1;
      datum += prime;
    }
    dgctl_release_write(dg_buf);
  }
}

int main(int argc, char* argv[])
{
  size_t size = 512 * 1024;
  int count = 3;
  void* newdata = malloc(size);
  dg_buffer* dg_buf = dgctl_create(size, count);
  while (1) // (select(db_buf->fd))
  {
    dgctl_lock(dg_buf);
    usleep(120 * 1000);
    dgctl_release(dg_buf);
  {
  dgctl_destroy(dbg_buf);
}
