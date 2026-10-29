> ⚠️ I may introduce breaking changes between minor version despite my best efforts not to. As the tool mature so will the API.

The scripting engine is powered by [LuaJIT](https://luajit.org/) (Lua 5.1 with some extensions).

Standard Lua libraries available are:

  - Package
  - Basic
  - Table
  - String
  - Math
  - bit (LuaJIT)   
  - [jit](https://luajit.org/ext_jit.html) (LuaJIT)
  - [ffi](https://luajit.org/ext_ffi.html) (LuaJIT)

Some standard libraries are not enabled by design!<br />
`goto` and `::label::` statements from Lua 5.2 are supported.<br />

# API Summary

- 🌐 [Globals](#-globals):
  + sleep: suspends execution until interval elapses
  + console: prettier print
  + Array: Lua table search
  + Failure: Structured error
  
- 📦 [Modules](#-modules):
  + [process](#-process) : get info about the current process.
  + [memory](#-memory)

## 🌐 Globals

### `sleep(ms: int)`

Suspends the execution of the Lua engine until the time-out interval elapses (interval is in milliseconds).

### `console: SetFuncs`

  + `log(any, ...)`
  + `warn(any, ...)`
  + `error(any, ...)`
  
Convenience methods to print value or array with timestamp and log level. Values are colored depending on their type.

💡 `print()` is an alias to `console.log()`

### `Array: SetFuncs`

  + `find(table, func) any`
  + `some(table, func) bool`
  + `includes(table, any) bool`
  
Convenience methods to search ~~array~~ Lua table.

Example: 

```lua
local arr = {1, 2, 3, 4, 5}

Array.find(arr, function(x) return x > 3 end)
Array.includes(arr, 3)

local arr = {
  {foo = "bar", value = 1},
  {foo = "baz", value = 2}
}

Array.find(arr, function(x) return x.foo == "bar" end)
Array.some(arr, function(x) return x.foo == "baz" end)
```

### `Failure(code?: string, message?: string) Failure{ code: string, message: string }`

Failure is a custom type (_userdata_) that represents an "error object" with an associated error code and message.
This provides a structured way to handle error.

- `code?: string` ("ERR_UNKNOWN")
  Error code.
  
- `message?: string` ("An unknown error occurred")
  Error message.
  
💡 `Failure` has a `__tostring` metamethod. If not invoked automatically, you can explicitly call it using `tostring(Failure)`

Example:

```lua
local err = Failure("ERR_NOT_FOUND", "The requested item was not found")
print(err.code)    -- "ERR_NOT_FOUND"
print(err.message) -- "The requested item was not found"
print(err)         -- "[ERR_NOT_FOUND]: The requested item was not found"

local value, err = Foo()
if err and err.code == "ERR_UNKNOWN" then
  error(err.message) -- Raise an error "An unknown error occurred"
  -- or
  error(tostring(err))
end
```

## 📦 Modules

### `📦 Process`

This is a module to get info about the current process (Mini-Launcher) and the target process to start.

```lua
local process = require("process")
```

- `pid: number` : process id
- `bin: string`: process file name
- `dir: string`: process parent dir
- `cwd: string`: process current working dir
- `args: string[]`: process arguments
- `env: { key: string, ... }` : process env. var. as key:value pairs
