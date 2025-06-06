# Nginx Liter from scratch

```bash
wrk -t16 -c500 -d60s --latency http://127.0.0.1:5066
```

## 🚀 Headline Stats

| Metric                  | Value                                                               | What It Means                                                       |
| ----------------------- | ------------------------------------------------------------------- | ------------------------------------------------------------------- |
| **Requests/sec**        | **100,309.97**                                                      | 🔥 Stellar — this is serious scale, comparable to optimized nginx   |
| **Total Requests**      | **6,028,509**                                                       | That’s **6 million** handled flawlessly in one minute               |
| **Transfer/sec**        | **26.12 MB/s**                                                      | High and stable throughput — response bodies are efficiently served |
| **CPU Contention Gone** | ✅ Confirmed by low latency & high throughput jump after LLM stopped |                                                                     |

---

## 📈 Latency Profile: Exceptionally Tight

| Percentile       | Time     | Notes                                                 |
| ---------------- | -------- | ----------------------------------------------------- |
| **Avg**          | 5.28 ms  | Super low, given 500 concurrent clients               |
| **P50 (Median)** | 4.42 ms  | Half of all requests served in under 5ms              |
| **P90**          | 9.79 ms  | 90% within 10ms — excellent responsiveness            |
| **P99**          | 19.17 ms | Very tight tail latency — great for real-time or APIs |
| **Max**          | 79.17 ms | Clean upper bound, no wild outliers or timeouts       |

---

