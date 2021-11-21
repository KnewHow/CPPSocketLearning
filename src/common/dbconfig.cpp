#include "dbconfig.h"

#include <fstream>
#include <unordered_map>

#include "glog/logging.h"

namespace server_client {
std::string DBConfig::config = "";
std::string DBConfig::charset = "";

DBConfig::DBConfig(const std::string &properties) {
    std::ifstream in;
    in.open(properties, std::ifstream::in);
    if(in.fail()) {
        LOG(ERROR) << "can't open propertiese file: " << properties;
        return;
    }
    std::unordered_map<std::string, std::string> db_properties;
    std::string line;
    while(!in.eof()) {
        std::getline(in, line);
        int index = line.find_first_of("=");
        std::string key = std::string(line.begin(), line.begin() + index);
        std::string value = std::string(line.begin() + index + 1, line.end());
        db_properties.insert({key, value});
    }
    
    config += ("host=" + db_properties["host"]); 
    config += (" user=" + db_properties["user"]);
    config += (" password=" + db_properties["password"]);
    config += (" dbname=" + db_properties["dbname"]);
    config += (" port=" + db_properties["port"]);
    if(db_properties.find("charset") != db_properties.end()) {
        charset = db_properties["charset"];
    } else {
        charset = "utf8";
    }
    
    LOG(INFO) << "config:" << config;
}

} // namespace server_client