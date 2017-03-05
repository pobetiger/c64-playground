#include <stdio.h>
#include <string.h>

#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

struct node {
        int data;
        struct node *next;
};

#define LL_HEAD(varname) \
        struct node varname = {0, NULL}
LL_HEAD(_root);



void changeString(char *src, int length)
{
        char *end = src+length-1;
        while (length--)
        {
                *src++ = *end--;
        }
}

static char err_buf[100];
void readwrite()
{
        char buf[] = "this is a test";
        int nbr=0;
        char buf2[50];
        char rd_buf[2];
        int idx = 0;

        // write test
        int fd = open("testfile", O_WRONLY | O_CREAT);
        if (!fd) {
                perror(err_buf);
                printf("Error: %s\n", err_buf);
        } else {
                printf("writing %d bytes to file\n", sizeof(buf));
                write(fd, buf, sizeof(buf));
                close(fd);
        }

        // read test
        fd = open("testfile", O_RDONLY);
        if (!fd) {
                perror(err_buf);
                printf("Error: %s\n", err_buf);
        } else {
                printf("reading from file\n");
                while (true) {
                        nbr = read(fd, rd_buf, 1);
                        if (nbr==0) break;

                        buf2[idx++] = rd_buf[0];

                        if (idx>=50) break;
                }

                printf("read complete, count=%d\n", idx);
                printf("data:\n%s\n", buf2);
        }
        

}


int main(int argc, char** argv)
{
        char str[] = "Hello Commodore 64";

        printf("Before change:\n");
        printf("%s\n", str);

        changeString(str, strlen(str));
        printf("After change:\n");
        printf("%s\n", str);

        printf("\n\n***** waiting some time *****\n\n");
        printf("\nSleep for 3 seconds...");
        sleep(3);
        printf(" OK\n");
        
        printf("Testing read/write operations\n");
        readwrite();

        return 0;
}

