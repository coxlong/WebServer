set(webserver_http
    HttpOps.cc
    HttpUtils.cc
    HttpRequest.cc
    HttpResponse.cc
)

add_library(webserver_http ${webserver_http})