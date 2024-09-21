#include<stdio.h>
unsigned char LEDs;
int LED_function() {
        while(LEDs != 0x80) {
                LEDs <<= 1;
                printf("%d ", LEDs);
        }

        while(LEDs != 0x1) {
                LEDs >>= 1;
                printf("%d ", LEDs);
        }

}
int main(void) {
        LEDs = 0x1;
        printf("%d ", LEDs);
        for(int i=0; i<4; i++) {
                LED_function();
        }
        printf("\n");
}
