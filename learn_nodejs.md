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
5. use `require` to import module
```Javascript
var logger = require('./logger');
```

better to use in recent version:

```Javascript
const logger = require('./logger');
```
6. use `jshint` to get sugestions on modifying JS file.
7. Module wrapper function is created automatically for each module.
