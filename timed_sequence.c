#include <6502.h>
#include <unistd.h>
#include <stdio.h>
#include <peekpoke.h>

char ch = 'A';


unsigned char tick_counter = 0;
unsigned char tick_timeout = 0;
unsigned char onTimeout(void)
{
        ++tick_counter;
        if (tick_counter >  tick_timeout) {
            if (ch=='A')
                    ch = 'B';
            else
                    ch = 'A';
        }

        (void) PEEK(0xdd0d); // clr irq
        return IRQ_HANDLED;
}

char ptStack[512];
const size_t nStackSize = 512;

void enableTimer(unsigned short timeout, irq_handler pf_handler)
{
        unsigned short handler = (unsigned short) &pf_handler;

        SEI();
        
        // set irq callback
        set_irq(pf_handler, ptStack, nStackSize);

        // allow stop to work
        POKE(0x0328, 0xe2);  // stop addr (lo)
        POKE(0x0329, 0xfc);  // stop addr (hi)

        tick_timeout = timeout;

        // set timer count to 10msec tick
        POKE(0xdd04, 10000      & 0xff); // lo
        POKE(0xdd05, (10000>>8) & 0xff); // hi

        POKE(0xdd0d, 0x81); // enable timerA
        POKE(0xdd0e, 0x91); // continous mode, 50Hz
        (void) PEEK(0xdd0d); // clr irq

        CLI();
}

int main(int argc, char**argv)
{
        char *junk=argv[0];
        argc++;
        
        printf("Enabling Timer A\n");
        enableTimer(110, onTimeout); // timeout at 1.1 sec

        // forever loop
        printf("Starting main loop\n");
        for (;;) {
                printf("%c", ch);
                sleep(1);
        }

        return 0;
}
