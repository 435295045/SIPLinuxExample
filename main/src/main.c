#include <unistd.h>
#include "sip/sip_handle.h"

int main()
{
    // 初始化
    init();
    int i = 0;
    while (1)
    {
        i++;
        sleep(1);
        if (i == 3)
        {
            callIP("192.168.1.199");
        }
    }

    return 0;
}