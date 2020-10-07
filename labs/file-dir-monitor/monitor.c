#define _XOPEN_SOURCE 500
#include <stdio.h>
#include "logger.h"
#include <stdlib.h>
#include <ftw.h>
#include <string.h>
#include <stdint.h>
#include <sys/inotify.h>
#include <stdio.h>
#include <unistd.h>

#define BUF_LEN (10 * (sizeof(struct inotify_event) + 100 + 1))
#define MAXLEVEL  2
int fd, numRead;
char buf[BUF_LEN] __attribute__ ((aligned(8)));
struct inotify_event *event;

static int display_info(const char *fpath, const struct stat *sb,
            int tflag, struct FTW *ftwbuf)
{
   // if the depth is above the max sub dirs, continue to next file
   if (ftwbuf->level > MAXLEVEL) {
       return 0; 
   }
    if(tflag == FTW_D || tflag == FTW_F){
        inotify_add_watch(fd, fpath, IN_CREATE | IN_MOVED_FROM | IN_DELETE);
    }
    

   return 0;           
}


int main(int argc, char **argv){
    int len, i = 0;
    char buffer[BUF_LEN], root[1024];
    if(argc == 2){
        infof("Starting File/Directory Monitor on %s\n", argv[1]);
        infof("-----------------------------------------------------\n");
        fd = inotify_init();
        if (nftw(argv[1], display_info, 20,0) == -1) {
            errorf("nftw");
            exit(0);
        }
        while(1){
            i=0;
            len = read(fd, buf, BUF_LEN);
            if (len == 0)
                errorf("read() from inotify fd returned 0!");

            if (len == -1)
                errorf("read");

            /* Process all of the events in buffer returned by read() */
            while(i<len){
                
            }
            for (char *p = buf; p < buf + numRead; ) {
                event = (struct inotify_event *) p;
                if (event->mask & IN_CREATE) {
                    if (event->mask & IN_ISDIR) {
                        infof("- [Directory - Create] - %s\n", event->name);
                    }
                    else
                        infof("- [File - Create] - %s\n", event->name);
                }
                
                if (event->mask & IN_MOVED_FROM) {
                    if (event->mask & IN_ISDIR)
                        infof("- [Directory - Modified] - %s\n", event->name);
                    else
                        infof("- [File - Modified] - %s\n", event->name);
                }
                
                if (event->mask & IN_DELETE) {
                    if (event->mask & IN_ISDIR)
                        infof("- [Directory - Delete] - %s\n", event->name);
                    else
                        infof("- [File - Delete] - %s\n", event->name);
                }

                p += sizeof(struct inotify_event) + event->len;
            }
        }
        (void) close(fd);
        
    }else{
        errorf("ERROR Invalid number of arguments %d\n", 2);
    }
    return 0;
}
