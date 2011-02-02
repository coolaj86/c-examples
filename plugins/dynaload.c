#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

/*
void speak(void)
{
  puts("main has spoken");
}
*/

int
main(int argc, char* argv[])
{
  void* a_handle;
  void* b_handle;

  void (*a_fn)(void);
  void (*b_fn)(void);
  void (*d_fn)(void);
  void (*n_fn)(void);
  void (*o_fn)(void);

  a_handle = dlopen("liba.so", RTLD_LAZY | RTLD_GLOBAL);
  if (!a_handle) 
  {
    fprintf(stderr, "%s\n", dlerror());
    exit(1);
  }

  b_handle = dlopen("libb.so", RTLD_LAZY | RTLD_GLOBAL);
  if (!b_handle) 
  {
    fprintf(stderr, "%s\n", dlerror());
    exit(1);
  }

  a_fn = dlsym(a_handle, "speak");
  b_fn = dlsym(b_handle, "speak");

  /*
    Loads either the 'speak' referenced above
    or the speak defined as global
  */
  d_fn = dlsym(RTLD_DEFAULT, "speak");
  n_fn = dlsym(RTLD_NEXT, "speak");
  if (!n_fn)
  {
    fprintf(stderr, "%s\n", dlerror());
    exit(1);
  }
  o_fn = dlsym(RTLD_NEXT, "speak");


  //speak();

  (*a_fn)();
  (*b_fn)();
  (*d_fn)();
  (*n_fn)(); // SIGSEGV
//  (*o_fn)(); // SIGSEGV

  dlclose(a_handle);
  dlclose(b_handle);

/*
  (*a_fn)();
  (*b_fn)();
  (*d_fn)();
  (*n_fn)();
  (*o_fn)();
*/
  return 0;
}
