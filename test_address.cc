#include "tw/log.h"
#include "tw/address.h"
#include <vector>
#include <map>
tw::Logger::ptr s_logger = TW_LOG_ROOT();
void test01(){
    std::vector<tw::Address::ptr> addrs;
    TW_LOG_DEBUG(s_logger) << "begin";
    bool v = tw::Address::Lookup(addrs
        , "www.baidu.com", AF_INET);
    TW_LOG_DEBUG(s_logger) << "end";
    if(!v){
        TW_LOG_ERROR(s_logger) << "lookup error";
        return;
    }
    for(size_t i = 0; i < addrs.size(); ++i){
        TW_LOG_DEBUG(s_logger) << i << "-" << addrs[i]->toString();
    }
    return;
}

void test_iface(){
    std::multimap<std::string, std::pair<tw::Address::ptr, uint32_t> > results;
    TW_LOG_DEBUG(s_logger) << "begin";
    bool v = tw::Address::GetInterfaceAddresses(results);
    TW_LOG_DEBUG(s_logger) << "end";
    if(!v){
        TW_LOG_ERROR(s_logger) << "getinterfaceAddress error";
        return;
    }
    for(auto& i : results){
        TW_LOG_DEBUG(s_logger) << i.first << "-" << i.second.first->toString()
            << "-" << i.second.second;
    }
    return;
}
void test_IP(){
    auto addr = tw::IPv4Address::Create("127.0.0.1", 8888);
    if(addr){
        TW_LOG_DEBUG(s_logger) << addr->toString();
    }
    auto addr1 = tw::IPv6Address::Create("1234:23a3:3b33:3c33:3d33:2367:3020:8338", 4000);
    if(addr1){
        TW_LOG_DEBUG(s_logger) << addr1->toString();
    }else{
        TW_LOG_ERROR(s_logger) << "create ipv6address error";
    }
    return;
}

int main(){
    //test01();
    //test_iface();
    test_IP();
    return 0;
}