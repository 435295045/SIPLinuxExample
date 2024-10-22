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
            callIP("10.200.0.3");
        }
    }

    return 0;
}