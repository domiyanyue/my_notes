# System Design: High Level Trade-offs

## Performance vs Scalability

Quoting Amazon's Werner Vogels: 
*A service is said to be scalable if when we increase the resources in a system, 
it results in increased performance in a manner proportional to resources added. 
Increasing performance in general means serving more units of work, but it can also 
be to handle larger units of work, such as when datasets grow.* 

## Latency vs Throughput
**Latency** is the amount of time needed to perform a single task.
**Throughput** is number of tasks system can perfrom per unit of time.
Generally, we should aim for maximum throughput with acceptable latency.

## Availability vs Consistency
### CAP theorem
In a distributed computer system, you can only support two of the following guarantees:

**Consistency** - Every read receives the most recent write or an error  
**Availability** - Every request receives a response, without guarantee that it contains the most 
recent version of the information  
**Partition Tolerance** - The system continues to operate despite arbitrary partitioning due to 
network failures  
Networks aren't reliable, so you'll need to support partition tolerance.  
You'll need to make a software tradeoff between consistency and availability.
