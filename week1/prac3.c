#include <stdio.h>
int main() {
        int one_cnt = 0;
        int input;
        int value = 0;

        scanf("%d", &input);
        for (int i = 0; i < 8; i++) {
                if (input & (1 << i)) one_cnt++;
        }

        for (int j = 0; j < one_cnt; j++) {
                value |= (0x80 >> j);
        }

        printf("1의 개수? : %d\n", one_cnt);
        printf("value? : %d\n", value);

}
