#include <webserver/http/HttpUtils.h>

using namespace webserver;
using namespace webserver::http;

StatusMap http::sDescriptions{
    { S200, "OK" },
    { S400, "Bad Request" },
    { S404, "Not Found" }
};

StatusMap http::sCodes{
    { S200, "200" },
    { S400, "400" },
    { S404, "404" }
};


VersionMap http::vMap {
    { http11, "HTTP/1.1" },
    { unknown, "unknow" }
};