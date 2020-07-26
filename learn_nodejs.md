# Learn Node JS


1. Node is a C++ wrapper on Google V8 javascript engine.
2. Node by default is a single thread-ed asynchronized model.
3. In Node, every file is a module. module info can be checked:
```Javascript
console.log(module);
```
4. module object has a field "exports".
```Javascript
module.exports.<external name> = <var/func name>;
```
