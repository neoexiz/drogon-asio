#include <drogon/drogon.h>

#include <future>
#include <iostream>

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/tcp.h>
#endif

using namespace drogon;

int nth_resp = 0;

int main()
{
    {
        auto client = HttpClient::newHttpClient("http://www.baidu.com");
        client->setSockOptCallback([](int fd) {
            std::cout << "setSockOptCallback:" << fd << std::endl;
#ifdef __linux__
            int optval = 10;
            ::setsockopt(fd,
                         SOL_TCP,
                         TCP_KEEPCNT,
                         &optval,
                         static_cast<socklen_t>(sizeof optval));
            ::setsockopt(fd,
                         SOL_TCP,
                         TCP_KEEPIDLE,
                         &optval,
                         static_cast<socklen_t>(sizeof optval));
            ::setsockopt(fd,
                         SOL_TCP,
                         TCP_KEEPINTVL,
                         &optval,
                         static_cast<socklen_t>(sizeof optval));
#endif
        });

        auto req = HttpRequest::newHttpRequest();
        req->setMethod(drogon::Get);
        req->setPath("/s");
        req->setParameter("wd", "wx");
        req->setParameter("oq", "wx");

        for (int i = 0; i < 10; ++i)
        {
            client->sendRequest(
                req, [](ReqResult result, const HttpResponsePtr &response) {
                    if (result != ReqResult::Ok)
                    {
                        std::cout
                            << "error while sending request to server! result: "
                            << result << std::endl;
                        return;
                    }

                    std::cout << "receive response!" << std::endl;
                    // auto headers=response.
                    ++nth_resp;
                    std::cout << response->getBody() << std::endl;
                    auto cookies = response->cookies();
                    for (auto const &cookie : cookies)
                    {
                        std::cout << cookie.first << "="
                                  << cookie.second.value()
                                  << ":domain=" << cookie.second.domain()
                                  << std::endl;
                    }
                    std::cout << "count=" << nth_resp << std::endl;
                });
        }
    }

    app().run();
}
