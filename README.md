
# Purpose
This repository demonstrates how to build and run a C++ server inside a Docker container. It includes examples of sending messages using curl, building and running the Docker container, and performing load tests using Locust and Apache Benchmark (ab).

The Apache Benchmark(ab) test was conducted with 1000 requests and got 13617 RPS(Requests per second). 

## send message by curl
```sh
curl -X POST http://localhost:8090/send -H "Content-Type: application/json" -d '{"message": "Your message here"}'
```

### docker build
```sh
cd cpp-api
docker build -t cpp-api .
docker run -d -p 8090:8090 cpp-api
```

#### load test using Locust 
```sh
python -m locust -f locustfile.py
 ```

 ### load test using ab
```sh
 ab -n 1000 -c 10 -p post_data.json -T application/json http://localhost:8090/send
 ```

#### result
```sh
 $ ab -n 1000 -c 10 -p post_data.json -T application/json http://localhost:8090/send

This is ApacheBench, Version 2.3 <$Revision: 1903618 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 100 requests
Completed 200 requests
Completed 300 requests
Completed 400 requests
Completed 500 requests
Completed 600 requests
Completed 700 requests
Completed 800 requests
Completed 900 requests
Completed 1000 requests
Finished 1000 requests


Server Software:        
Server Hostname:        localhost
Server Port:            8090

Document Path:          /send
Document Length:        36 bytes

Concurrency Level:      10
Time taken for tests:   0.073 seconds
Complete requests:      1000
Failed requests:        0
Total transferred:      133000 bytes
Total body sent:        179000
HTML transferred:       36000 bytes
Requests per second:    13617.67 [#/sec] (mean)
Time per request:       0.734 [ms] (mean)
Time per request:       0.073 [ms] (mean, across all concurrent requests)
Transfer rate:          1768.70 [Kbytes/sec] received
                        2380.43 kb/s sent
                        4149.13 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.1      0       0
Processing:     0    1   0.1      1       1
Waiting:        0    0   0.1      0       1
Total:          0    1   0.1      1       1
```