#include <Configuration.hpp>


NetworkConfiguration Configuration::network;
OriginConfiguration Configuration::origin;
StyleConfiguration Configuration::style;


void Configuration::ReadAll(void){
    network.Read();
    origin.Read();
    style.Read();
}

