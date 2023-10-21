#include "tw/log.h"
#include "tw/bytearray.h"
#include "tw/macro.h"
static tw::Logger::ptr s_logger = TW_LOG_ROOT();
void test(){
    std::vector<int8_t> vec;
    for(int i = 0; i < 100; ++i){
        vec.push_back(rand());
    }
    tw::ByteArray::ptr ba(new tw::ByteArray(1));
    for(auto& i : vec){
        ba->writeFint8(i);
    }
    ba->setPosition(0);
    for(size_t i = 0; i < vec.size(); ++i){
        int8_t v = ba->readFint8();
        TW_ASSERT(v == vec[i]);
    }
    TW_ASSERT(ba->getReadSize() == 0);
    TW_LOG_INFO(s_logger) << "writeFint8 / readFint8 "
        << ba->getSize();
}
int main(){
    test();
    return 0;
}