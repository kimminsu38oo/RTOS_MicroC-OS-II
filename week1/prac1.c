#include<stdio.h>
int LED_function(int input) {
                return input << 1;
}
int main(void) {
                int return_value;
                return_value = LED_function(2);
                printf("return_value:%d\n", return_value);
}
