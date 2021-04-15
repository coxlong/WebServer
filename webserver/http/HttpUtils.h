#pragma once

#include <unordered_map>
#include <string>

namespace webserver {
namespace http {

enum Method {
    GET, POST, Invalid
};
enum Version {
    http11, unknown
};

enum HttpStatusCode {
    S200, S404, S400, S500
};

using StatusMap=std::unordered_map<HttpStatusCode, std::string>;
using SSMap=std::unordered_map<std::string, std::string>;
using VersionMap=std::unordered_map<Version, std::string>;

extern StatusMap sDescriptions;
extern StatusMap sCodes;
extern VersionMap vMap;
extern SSMap cTypeMap;
extern char favicon[555];
extern const std::string wwwroot;

#define CRLF "\r\n"
#define SPACE " "
#define COLON ":"

std::string getCurTime();

}
}