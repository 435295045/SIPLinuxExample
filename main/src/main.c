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
            // 这里呼叫多个
            callIP("10.200.0.5");
            callIP("10.200.0.4");
        }
    }

    return 0;
}