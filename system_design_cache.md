# System Design : Caching

The primary use of a cache is to speed up computation by exploiting patterns presents
in query streams. Since the access of RAM is orders of magnitude faster than access to
secondary memory (disk), the average latency drops significantly with the use of a cache.
A secondary goal is reducing workload on backend servers.

