#include <iostream>
#include <string>
int main(){
    uint64_t timer(time(0));
    struct tm tm;
    time_t t = timer;
    std::cout << t << " ||" << timer << std::endl;
    localtime_r(&t, &tm);
    std::cout << t << " ||" << timer << std::endl;
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    std::cout << buf << std::endl;

}