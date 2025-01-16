#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;

class ThreadPool {
public:
    ThreadPool(size_t num_threads) : stop(false) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this] {
                            return stop || !tasks.empty();
                        });
                        
                        if (stop && tasks.empty()) {
                            return;
                        }
                        
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }
    
    template<class F>
    void enqueue(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace(std::forward<F>(f));
        }
        condition.notify_one();
    }
    
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers) {
            worker.join();
        }
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

int main() {
    // Create thread pool with number of threads equal to hardware concurrency
    auto num_threads = std::thread::hardware_concurrency();
    std::cout << "Number of threads: " << num_threads << std::endl;
    ThreadPool pool(num_threads);
    
    httplib::Server svr;
    std::atomic<int> api_call_count{0};
    std::mutex cout_mutex;  // Mutex for protecting cout

    svr.Post("/send", [&](const httplib::Request& req, httplib::Response& res) {
        pool.enqueue([&, req_body = req.body]() {
            try {
                auto json_body = json::parse(req_body);
                std::string message_body = json_body.at("message").get<std::string>();
                
                {
                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << message_body << std::endl;
                }
                
                // Note: Response needs to be set in the main thread as httplib
                // expects this
                res.set_content("Message sent: " + message_body + "\n", "text/plain");
            } catch (const json::exception& e) {
                res.status = 400;
                res.set_content("Invalid JSON: " + std::string(e.what()) + "\n", "text/plain");
            }
        });
    });

    svr.Get("/count", [&](const httplib::Request& req, httplib::Response& res) {
        int current_count = api_call_count++;
        res.set_content("API call count: " + std::to_string(current_count) + "\n", "text/plain");
    });

    std::cout << "Server started at http://localhost:8090 with " << num_threads << " worker threads" << std::endl;
    svr.listen("0.0.0.0", 8090);

    return 0;
}