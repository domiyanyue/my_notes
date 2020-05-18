# API design: RPC, REST, GraphQL

It's been around for a long time. 
The big idea or RPC is to call a function on another server.

## RPC (Remote Procedure Call) Examples
```
GET /listConversations
GET /listMessages?id=123
POST /sendMessage?id=123
```

```
POST /sendMessage?id=2
{
  "body" : "Hello"
}
```
### Advantage of RPC
1. Simple and easy to understand.
2. Lightweight paylodas
3. High performance

### Disadvantage of RPC
1. Tight coupling with the system, including details on underlying system like how to name functions.
2. No discoverability (hard to for beginners to start).
3. Function explosion.
```
listConversation
listConversationV2
getMessage
getMessage2
```

## REST (Representational State Transfer)

Representational state transfer (REST) is a *software architectural style* that defines a set of
constraints to be used for creating Web services.
REST != JSON over HTTP
REST != RESTful procesdure call

The fundamental unit is resource. 
*"Here are some resources and what you can do with them?"*

### REST API Example

```
GET https://api.example.io/
{
  "converions": {
    "href" : "https://api.io/conversations",
    "rel": ["collection"],
    "desc": "View all conversations"
  },
  "messages" : {
    "href": "https://api.io/messages",
    "rel": ["collection"]
  }
}
```

### Advantages of REST
1. Decoupled client and server
2. API can evolve over time
3. Reuses HTTP

### Disadvantage of REST
1. No single spec
2. Big payloads and "chattiness"

## GraphQL
Instead of modeling functions (RPC) or resources (REST)
