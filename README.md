# Redis Clone – C++ Server & Client Implementation using RESP2

## Overview

This project is a custom Redis-like in-memory key-value store implemented in modern C++. It features both a server and client built from scratch, communicating over sockets using the [RESP2 (REdis Serialization Protocol v2)](https://redis.io/docs/reference/protocol-spec/).

The server supports a range of Redis-compatible commands including string, list, and hash operations. A custom command `lget` was also added to retrieve the contents of a list.

---

## Features

- Fully functioning Redis server and client using C++ and sockets.
- Implements RESP2 protocol for communication.
- Key Redis commands: `SET`, `GET`, `DEL`, `EXPIRE`, `FLUSHALL`, and more.
- List operations: `LPUSH`, `RPUSH`, `LPOP`, `RPOP`, `LLEN`, etc.
- Hash operations: `HSET`, `HGET`, `HDEL`, etc.
- Custom command: `LGET` – fetch list contents by key.
- Modular design with clear separation between CLI, network, database, and command handling.

---

## Project Structure

```
RedisClone/
│
├── Server/
│   ├── main.cpp
│   ├── RedisServer.cpp / RedisServer.hpp
│   ├── RedisCommandHandler.cpp / RedisCommandHandler.hpp
│   └── RedisDatabase.cpp / RedisDatabase.hpp
│
└── Client/
    ├── main.cpp
    ├── CLI.cpp / CLI.hpp
    ├── RedisClient.cpp / RedisClient.hpp
    ├── CommandHandler.cpp / CommandHandler.hpp
    └── ResponseParser.cpp / ResponseParser.hpp
```

---

## Server Files Explanation

### `main.cpp`
- Entry point of the Redis server application.
- Initializes and starts the `RedisServer` instance.

### `RedisServer.cpp / .hpp`
- Handles socket setup, accepting client connections, and managing client threads.
- Listens for incoming RESP2 messages from clients and delegates them for processing.

### `RedisCommandHandler.cpp / .hpp`
- Parses and executes commands received from clients.
- Supports all implemented Redis and LGET custom command by invoking appropriate functions from `RedisDatabase`.

### `RedisDatabase.cpp / .hpp`
- Core in-memory data store and logic for key-value, list, and hash data types.
- Implements TTL expiration, list manipulations, and hash maps.
- Performs thread-safe operations on data.

---

## Client Files Explanation

### `main.cpp`
- Entry point of the Redis client application.
- Initializes the CLI and connects to the server.

### `CLI.cpp / .hpp`
- Provides a user-friendly command-line interface.
- Reads user input and sends it to the `CommandHandler`.

### `CommandHandler.cpp / .hpp`
- Converts CLI input into RESP2 messages.
- Sends commands to the server via the `RedisClient`.

### `RedisClient.cpp / .hpp`
- Manages socket communication with the Redis server.
- Sends raw RESP2 commands and receives server responses.

### `ResponseParser.cpp / .hpp`
- Parses RESP2 responses from the server into human-readable output.
- Handles different response types: bulk strings, arrays, integers, errors, etc.

---

## RESP2 Protocol

All client-server communications follow the RESP2 protocol. It supports:

- `+` Simple Strings
- `-` Errors
- `:` Integers
- `$` Bulk Strings
- `*` Arrays

Example of a `SET` command in RESP2:

```
*3
$3
SET
$3
foo
$3
bar

```

---

## Supported Commands

### Server Management

| Command    | Description |
|------------|-------------|
| `FLUSHALL` | Removes all keys from all databases. Equivalent to clearing the in-memory store completely. |

---

### String Commands

| Command | Description |
|--------|-------------|
| `SET key value` | Stores the `value` under `key`. Overwrites if key exists. |
| `GET key`       | Retrieves the value of `key`. Returns `nil` if key doesn't exist. |
| `DEL key`       | Deletes the specified key. Returns 1 if deleted, 0 if not found. |
| `TYPE key`      | Returns the data type of the value stored at `key`: string, list, hash, etc. |
| `EXPIRE key seconds` | Sets a time-to-live (TTL) in seconds for the specified key. |
| `RENAME key newkey` | Renames a key to `newkey`, overwriting the destination if it exists. |

---

### List Commands

| Command          | Description |
|------------------|-------------|
| `LPUSH key val`  | Pushes `val` to the left (head) of the list stored at `key`. |
| `RPUSH key val`  | Pushes `val` to the right (tail) of the list. |
| `LPOP key`       | Removes and returns the first element of the list. |
| `RPOP key`       | Removes and returns the last element of the list. |
| `LLEN key`       | Returns the length of the list stored at `key`. |
| `LREM key count val` | Removes `count` occurrences of `val` from the list. |
| `LINDEX key index` | Retrieves the element at position `index`. Negative indices allowed. |
| `LSET key index val` | Sets the list element at `index` to `val`. |
| `LGET key`       | **Custom** command: Returns all elements in the list at `key`. Useful for inspecting entire list contents. |

---

### Hash Commands

| Command          | Description |
|------------------|-------------|
| `HSET key field val` | Sets the field in hash `key` to `val`. Creates hash if not exists. |
| `HGET key field`     | Gets the value associated with `field` in hash `key`. |
| `HEXISTS key field`  | Returns 1 if `field` exists in the hash, 0 otherwise. |
| `HDEL key field`     | Deletes the specified field from hash `key`. |
| `HGETALL key`        | Returns all fields and values of the hash as an array. |
| `HKEYS key`          | Returns all field names in the hash. |
| `HVALS key`          | Returns all values in the hash. |
| `HMSET key field1 val1 [field2 val2 ...]` | Sets multiple field-value pairs in a single command. |

---

## Example Usage

```bash
> SET mykey hello
OK

> GET mykey
hello

> LPUSH fruits apple
1

> RPUSH fruits banana
2

> LGET fruits
apple
banana

> HSET user name John
1

> HGET user name
John

> HGETALL user
name
John
```

---

## How to Run

### Build Server

```bash
cd Redis_Server
make
./bin/my_redis_server
```

### Build Client

```bash
cd Redis_Client
make
./bin/my_redis_cli
```

---
