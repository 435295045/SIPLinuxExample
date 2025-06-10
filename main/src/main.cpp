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
        if (i == 5)
        {
            // 3s后测试呼叫
            std::map<std::string, std::string> headers;
            // 这里呼叫多个
            sip::call("RM-1-1-1-1-1-9", headers);
            //sip::callIP("192.168.1.199", headers);
        }
    }

    return 0;
}