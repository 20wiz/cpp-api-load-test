// #include <aws/core/Aws.h>
// #include <aws/sqs/SQSClient.h>
// #include <aws/sqs/model/SendMessageRequest.h>
#include <iostream>
#include "httplib.h"
#include "json.hpp"  // Include the nlohmann/json library

using json = nlohmann::json;

int main() {

    httplib::Server svr;

    svr.Post("/send", [&](const httplib::Request& req, httplib::Response& res) {
            auto json_body = json::parse(req.body);
            std::string message_body = json_body.at("message").get<std::string>();
            std::cout << message_body << std::endl;
            // send_message(queue_url, message_body);
            res.set_content("Message sent: " + message_body + "\n", "text/plain");
    });

    int api_call_count = 0;

    svr.Get("/count", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_content("API call count: " + std::to_string(api_call_count) + "\n", "text/plain");
        api_call_count++;
    });

    std::cout << "Server started at http://localhost:8090" << std::endl;
    // svr.listen("localhost", 8090); not working for ssh port forwarding
    svr.listen("0.0.0.0", 8090);

    return 0;
}