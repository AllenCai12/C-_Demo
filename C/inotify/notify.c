#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/stat.h>


#include <unistd.h>

static int s_running;

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 *( EVENT_SIZE + 16 ) )
void *_check_event_thread(void *arg)
{
    int length, i = 0;
    int fd;
    int wd;
    char buffer[BUF_LEN];

    fd = inotify_init();

    if ( fd < 0 ) {
        perror( "inotify_init" );
    }

    mkdir("/tmp/loglevel", 0777); 
    wd = inotify_add_watch( fd, "/tmp/loglevel/",  IN_MODIFY | IN_CREATE | IN_DELETE);
    if (wd < 0) {
        perror("fail to add watch");
        return NULL;
    } 
    fd_set rfds;

    /* Wait until an event happens or we get interrupted 
       by a signal that we catch */
      struct  timeval time_out_val = {2, 0};
    
    while(s_running) {
      FD_ZERO (&rfds);
      FD_SET (fd, &rfds);
      time_out_val.tv_sec=2;
      time_out_val.tv_usec=0;
      int ret = select (fd+1, &rfds, NULL, NULL, &time_out_val);
      if (ret <= 0 ) {
          printf("time out: running:%d\n", s_running);
          continue;
      }

      printf("file create start read...\n");
      ret = read(fd, buffer, BUF_LEN);
      if (ret <0) {
          continue;
      }

      printf("file create...\n");
      
    }

}


int main( int argc, char **argv )
{
    
   pthread_t thread_check;
   s_running = 1;
   pthread_create(&thread_check, NULL, _check_event_thread, NULL);
   getchar();

   s_running = 0;
   pthread_join(thread_check, NULL);

    return 0;

}
