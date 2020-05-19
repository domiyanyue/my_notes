# System Design : Caching

The primary use of a cache is to speed up computation by exploiting patterns presents
in query streams. Since the access of RAM is orders of magnitude faster than access to
secondary memory (disk), the average latency drops significantly with the use of a cache.
A secondary goal is reducing workload on backend servers.

## Caching at Database Query Level
Hash the query as a key and store the result to the cache. This approach suffers from:
* Hard to delete a cached result with complex queries
* If one piece of data changes such as a table cell, need to delete all cached queries that may include the change

## Caching at Object Level
See your data as an object, similar to what you do with application code. 
