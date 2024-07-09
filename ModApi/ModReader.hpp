#ifndef MOD_READER
#define MOD_READER
#include <string>
#include <list>
#include <memory>
#include "ModApi/ApiStructs.h"
std::unique_ptr<std::list<AzurModInfo>> read_mods_info(std::string path);
#endif // !MOD_READER


