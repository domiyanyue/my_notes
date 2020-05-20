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

## When to update the cache

### Cache-aisde (Lazy-loading)
Only requested datat is cached, avoiding filling up the cache with datat that isn't required. 
Appplication is responsible for reading and writing from storage. Cache *does not interact* with storage directly. The
application does the following:
1. Look for entry in cache, resulting a cache miss
2. Load entry from the database
3. Add entry to cache
4. Return entry

Subsequent reads of data added to cache are fast. Cache-aside is also referred to as lazy-loading. Only requested data is cached. which avoids filling up the cace with data that ins't requested. 

#### Disadvatages of cache-aside
* Each cache miss results in three trips. which can cause a noticeable delay.
* 

### Write-throught
The application uses the cache as the main data storage, reading and writing data to it, while the cache is responsible for reading and writing to the database:
1. Application adds/updates entryes in the cache
2. Cache synchronously writes entry to data store
3. Return

Write-through is a slow overall operation due to write operation, but sebsequent reads of just written data are fast. Users are generatlly more tolerant of latency when updating data than reading data. Data in the cache is not stale. 

#### Disadvatages of cache-aside
* When a new node is created due to failure or scaling, the new node will not cache entries until the entry is updated in the database. 
Cache-aside in conjuction with write through can mitigate this issue.
* Most data written might never be read, which can be minimized with a TTL.

### Write-behind (write-back)


