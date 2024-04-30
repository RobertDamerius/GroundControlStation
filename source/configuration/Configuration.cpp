#include <Configuration.hpp>


GCSConfiguration Configuration::gcs;


void Configuration::ReadAndSaveAll(void){
    // read
    gcs.Read();

    // save
    gcs.Save();
}

