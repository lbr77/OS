#include "../sys.h"

int main() {
    for(int i=0;i<=10;i++) {
        puts("Program[1] print1\n");
        int64_t w = get_time() + 10;
        while(get_time() < w) yield();
    }
    return 0;
}