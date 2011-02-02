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

  a_handle = dlopen("liba.so", RTLD_LAZY | RTLD_LOCAL);
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
    Loads either the 'speak' referenced in this same file
    or, if that is commented out, it loads the globally loaded `speak`

    If both are loaded globally, the first is used
  */
  d_fn = dlsym(RTLD_DEFAULT, "speak");
  n_fn = dlsym(RTLD_NEXT, "speak");
  if (!n_fn)
  {
    // this seems to always fail. I don't understand why
    fprintf(stderr, "%s\n", dlerror());
    // exit(1);
  }


  //speak();

  (*a_fn)();
  (*b_fn)();
  (*d_fn)();
//  (*n_fn)(); // SIGSEGV

  dlclose(a_handle);
  dlclose(b_handle);

/*
  // all of these will SIGSEGV since they have been unloaded
  // There is, however, an option which prevents unloading
  // of specific functions
  (*a_fn)();
  (*b_fn)();
  (*d_fn)();
  (*n_fn)();
*/
  return 0;
}
