#include <string>
#ifndef API_STRUCTS
#define API_STRUCTS

typedef struct AzurModInfo {
	std::string modname;
	std::string version;
	std::string author;
	std::string mod_path;
	std::string mod_description;
} AzurModInfo;

#endif // !API_STRUCTS
