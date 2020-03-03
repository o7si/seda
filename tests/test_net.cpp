#include <net/http.h>
#include <net/http_parser.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

using namespace o7si::net;

HttpResponse make_response()
{
    HttpResponse response;
    response.status(GenStatusFrom(200));    
    //response.reason_phrase(HttpStatusToString(response.status()));
    response.reason_phrase("test");
    response.version(GenVersionFrom("HTTP/1.1"));
    response.body(R"+(
    <html>
    <head>
        <link rel="icon" href="data:;base64,=">
    </head>
    <body>
        <a href="test.html">test</a>
    </body>
    </html>
    )+");

    return response;
}

int main(int argc, char* argv[])
{
    std::cout << "test_net" << std::endl;
    
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_sockaddr;
    memset(&server_sockaddr, 0, sizeof(sockaddr_in));
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(10000);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(server_fd, (sockaddr*)&server_sockaddr, sizeof(server_sockaddr));

    listen(server_fd, 128);
    
    sockaddr_in client_sockaddr;
    socklen_t client_sockaddr_len = sizeof(client_sockaddr_len);
    memset(&client_sockaddr, 0, sizeof(client_sockaddr));
    int client_fd = accept(server_fd, (sockaddr*)&client_sockaddr, &client_sockaddr_len);
    
    HttpResponse response = make_response();
    std::string format = response.format();
    // 发送网页数据
    write(client_fd, format.data(), format.size());
    
    while (true)
    {
        char buffer[BUFSIZ];
        int len = read(client_fd, buffer, BUFSIZ);
        //if (len == 0)
          //  break;
        std::cout << buffer << std::endl;    
        continue;
        HttpRequestParser parser;
        parser.parser_execute(buffer, len); 
        auto req = parser.inner_request();
        std::cout << "发送数据" << std::endl;
        std::cout << req->format() << std::endl;

    }

    close(client_fd);
    close(server_fd);  
    return 0;
}
