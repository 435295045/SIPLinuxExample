#include <unistd.h>
#include "sip/SipHandle.hpp"
#include "utils/AlsaUtils.hpp"

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
            //开启alsa
            ualsa::open();
            // 3s后测试呼叫
            std::map<std::string, std::string> headers;
            // 这里呼叫多个
            //sip::call("RM-1-1-1-1-1-6", headers);
            //sip::callIP("10.200.0.3", headers);
        }
    }

    return 0;
}