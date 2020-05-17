# Consistency Pattern in System Design

Withou multiple copies of the same data, we are facing the option of how to synchronize them so that clients have a consistent view of data.

## Weak Consistency


Client side is **ok with loss of data**. After a write, reads may or may not see it. A best effort approach is taken.  
**Weak consistency** works well in **real time use cases** such as VoIP, Video chat and real time multi player games. In those cases,
If you loss connection/data for a few seconds, when you regain connection, you don't get the data (voice/video frames) that was created
during connection loss.

## Eventual Consistency

fter a write, reads will evetuanlly see it.
Data is **replicated asynchronously**.  
This approach is seen in systems such as DNS and email. Eventual consistency works well in **highly available systems**.

### Eventual Consistency Example: DNS Server
DNS servers do not nessesarily refelct the lastest value. It takes a while to replicated modified value to all DNS clients and servers. 
It's proven to be extremely scalable and become one of the fundataion of Internet.

## Strong Consistency

After a write, reads will see it. Data is **replicated syncrhonously**.  
This approach is seen in file systems and RDMBSes. **Strong consistency** works well in **systems that need transactions**.

