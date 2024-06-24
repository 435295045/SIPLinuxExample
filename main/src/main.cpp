#include <unistd.h>
#include "sip/SipHandle.hpp"

int main()
{
    // 初始化
    sip::init();
    int i = 0;
    while (1)
    {
        i++;
        sleep(1);
        if (i == 3)
        {
            // 3s后测试呼叫
            std::map<std::string, std::string> headers;
            // 这里呼叫多个
            sip::callIP("10.200.0.5", headers);
            sip::callIP("10.200.0.4", headers);
        }
    }

    return 0;
}