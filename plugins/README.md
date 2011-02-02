dynamic library loading
====

This shows a simple `Hello World` example of how to develop a plugin (or middleware) style architecture in C.

In this example there are two libraries - `liba` and `libb`. They both define a method `speak`.

The relevant C functions used are `dlopen`, `dlsym`, and `dlclose`.

Note: OS X omits `-Wl` before `-soname`.
If compiling on Linux replace `-soname` with `-Wl,-soname` in the Makefile.

Example: lighttpd
----

`lighttpd` is a webserver which uses plugins (`mod_fastcgi`, `mod_compress`, `mod_auth`, etc).

`lighttpd` doesn't link against the plugins, but rather it dynamically loads them at runtime if they are configured in `/etc/lighttpd.conf` and they exist on the filesystem. For example `/usr/lib/mod_fastcgi.so`.

Example: rgbtoxyz
----

Let's imagine we have some camera that generates RAW (rgb) images and we want to convert them to more generally useful formats such as jpeg, png, etc.

We may have any number of libraries for handling, but the interface will probably be the same.

    int encode(void* inbuf, int width, int height, int depth, void* outbuf, int* length);

We could write a simple configuration file that looks something like this:

    jpeg: rgb16tojpeg.so
    png: rgb16topng.so
    // bmp: rgb16tobmp.so

`rgbtoxyz` should read the conf at startup (or when the SIGHUP signal is caught if it is acting as a server or daemon).

By default none of the modules are loaded, but when the configuration file is read the application will be set up to produce outputs in the various formats.

In this fashion, modules are only into memory if it will be used.

More importantly, if I come up with a new format I should be able to put the `rgb16toformatx.so` into the plugins directory and update the settings file and, voila, the output should suddenly also be encoded in the new format.

Resources
====

When I was initially trying to figure out how to do plugins in C I wasn't googling the correct terms.

Whereas in the higher level languages we know this technique as `plugins` or `middleware`, in C it is referred to as "dynamic loading".

The best to article I found on the subject is [Dynamic loading and un-loading of shared libraries using libdl](http://www.yolinux.com/TUTORIALS/LibraryArchives-StaticAndDynamic.html) (about halfway down the page).

Also, the man pages of [dlopen](http://pubs.opengroup.org/onlinepubs/009695399/functions/dlopen.html) and [dlsym](http://pubs.opengroup.org/onlinepubs/009695399/functions/dlsym.html) were quite helpful.

