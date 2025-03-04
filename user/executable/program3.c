#include "../sys.h"

int main() {
    for(int i=0;i<=10;i++) {
        puts("Program[3] print3\n");
        int64_t w = get_time() + 30;
        while(get_time() < w) yield();
    }
    return 0;
}