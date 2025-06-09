# Nginx Liter from scratch

 - logging has been disabled since its blocking 
## with blocking i/o log
```bash
wrk -t16 -c10000 -d60s --latency http://127.0.0.1:5066
```

# with non blocking i/o logging and sendfile

| Metric         | Small Response      | Big Response (`sendfile()`) |
| -------------- | ------------------- | --------------------------- |
| Requests/sec   | 551,900             | 52,710                      |
| Transfer/sec   | 143.69 MB/s         | **21.42 GB/s** 🔥           |
| Total Requests | 33.1 million        | 3.17 million                |
| Avg Latency    | 1.69ms              | 16.95ms                     |
| P99 Latency    | 7.86ms              | 80.47ms                     |
| Payload Size   | Tiny (<1KB)         | \~400–500KB                 |
| Focus          | Speed & concurrency | File-serving bandwidth      |
