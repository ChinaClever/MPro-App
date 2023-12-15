#include "keythread.h"


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>


int key_main(void)
{
    struct input_event key;
    int fd = open("/dev/input/event0", O_RDWR);
    if(fd < 0)
    {
        perror("open");
        return -1;
    }

    while(1)
    {
        if(read(fd, &key, sizeof(key)) > 0)
        {
            if(key.type == EV_KEY)
            {
                switch(key.code)
                {
                case KEY_UP:
                    printf("%d.%lds, up is %s\n",key.time.tv_sec, key.time.tv_usec, key.value?"press":"release");
                    break;
                case KEY_DOWN:
                    printf("%d.%lds, down is %s\n",key.time.tv_sec, key.time.tv_usec, key.value?"press":"release");
                    break;
                case KEY_MENU:
                    printf("%d.%lds, menu is %s\n",key.time.tv_sec, key.time.tv_usec, key.value?"press":"release");
                    break;
                }
            }
        }
    }
    close(fd);
    return 0;
}


KeyThread::KeyThread(QObject *parent)
    : QObject{parent}
{

}
