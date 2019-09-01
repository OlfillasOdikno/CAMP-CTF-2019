// align manually using:
// gcc buffer_read.c -o buffer_read.so -shared -fPIC  -Wall -DLINUX -ldl -falign-functions=16 -Wl,-Ttext=0xf40

#define _GNU_SOURCE

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <poll.h>

//
// originals
//
ssize_t (*original_read)(int, const void *, size_t);

__attribute__((constructor)) void preeny_buffer_read_orig()
{
        original_read = dlsym(RTLD_NEXT, "read");
        unsetenv("LD_PRELOAD");
}


ssize_t read(int fd, void *buf, size_t size)
{
        ssize_t x = 0;
        for(;;) {
                ssize_t res = original_read(fd, buf+x, size-x);
                if (res<0 || size <= 0)
                        return res;
                x += res;
                if (x == size)
                        return size;
        }
}

