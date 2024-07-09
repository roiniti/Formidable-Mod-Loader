#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "ModReader.hpp"
#include "logger/clogger.h"

#include <iostream>
#include <dirent.h>
#include <sys/stat.h>


std::map<std::string, std::string> parse_json(const std::string& json) {
    std::map<std::string, std::string> result;
    size_t pos = 0;

    while (pos < json.length()) {
        size_t key_start = json.find('"', pos);
        if (key_start == std::string::npos)
            break;

        size_t key_end = json.find('"', key_start + 1);
        if (key_end == std::string::npos)
            break;

        std::string key = json.substr(key_start + 1, key_end - key_start - 1);

        size_t colon_pos = json.find(':', key_end);
        if (colon_pos == std::string::npos)
            break;

        size_t value_start = json.find_first_not_of(" \t\n\r\f\v", colon_pos + 1);
        if (value_start == std::string::npos)
            break;

        size_t value_end = json.find_first_of(",}", value_start);
        if (value_end == std::string::npos)
            break;
        std::string value = "";

        if (json[value_start] == '"') {
            while (json[value_end] != '"') {
                value_end--;
            }
            value = json.substr(value_start + 1, value_end - value_start - 1);
        }
        else {
            value = json.substr(value_start, value_end - value_start);
        }

        
        pos = value_end + 1;

        result[key] = value;
    }

    return result;
}

std::string read_file(std::string path) {
    std::ifstream file(path);
    std::string content;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        file.close();
    }
    else {
        return "";
    }

    return content;
}

std::string findOrEmpty(std::map<std::string, std::string> map, std::string key) {
    auto iterador = map.find(key);
    if (iterador != map.end()) {
        return map[key];
    }
    return "";
}

std::unique_ptr<AzurModInfo> read_mod_info(std::string ruta) {
    std::string json_info=read_file(ruta+"/"+"modinfo.json");
    if (json_info == "")
        return nullptr;
    std::map<std::string, std::string> parsed_json = parse_json(json_info);

    std::unique_ptr<AzurModInfo> modinfo = std::make_unique<AzurModInfo>();

    modinfo->author = parsed_json["author"];
    modinfo->modname = parsed_json["mod-id"];
    modinfo->version = parsed_json["version"];
    auto iterador = parsed_json.find("source-dir");
    if (iterador != parsed_json.end()) {
        modinfo->mod_path = ruta + "/"+parsed_json["source-dir"];
    }
    else {
        modinfo->mod_path = ruta + "/luacode";
    }
    modinfo->mod_description = findOrEmpty(parsed_json, "description");
    return modinfo;
}


std::unique_ptr<std::list<AzurModInfo>> read_mods_info(std::string path) {
    std::unique_ptr<std::list<AzurModInfo>> return_list = std::make_unique<std::list<AzurModInfo>>();

    DIR* dir = opendir(path.c_str());
    if (dir == nullptr) {
        logMessage("modloader", "Cannot open mod directory");
        return nullptr;
    }
    struct dirent* entrada;

    while ((entrada = readdir(dir)) != nullptr) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue;
        }

        struct stat info;
        std::string rutaCompleta = std::string(path) + "/" + entrada->d_name;
        if (stat(rutaCompleta.c_str(), &info) != 0) {
            continue;
        }

        if (S_ISDIR(info.st_mode)) {
            std::unique_ptr<AzurModInfo> modinfo = read_mod_info(rutaCompleta.c_str());
            if (modinfo != nullptr)
                return_list->push_back(*modinfo);
        }
    }

    closedir(dir);

    if (return_list->empty()) {
        return nullptr; 
    }
    return return_list;


}