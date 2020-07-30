#include "handlers_internal.h"

void ddtrace_phpredis_handlers_startup(void) {
    // clang-format off
    ddtrace_string methods[] = {
        DDTRACE_STRING_LITERAL("append"),
        DDTRACE_STRING_LITERAL("auth"),
        DDTRACE_STRING_LITERAL("bgrewriteaof"),
        DDTRACE_STRING_LITERAL("bgsave"),
        DDTRACE_STRING_LITERAL("close"),
        DDTRACE_STRING_LITERAL("connect"),
        DDTRACE_STRING_LITERAL("decr"),
        DDTRACE_STRING_LITERAL("decrby"),
        DDTRACE_STRING_LITERAL("del"),
        DDTRACE_STRING_LITERAL("delete"),
        DDTRACE_STRING_LITERAL("dump"),
        DDTRACE_STRING_LITERAL("echo"),
        DDTRACE_STRING_LITERAL("exists"),
        DDTRACE_STRING_LITERAL("expire"),
        DDTRACE_STRING_LITERAL("expireat"),
        DDTRACE_STRING_LITERAL("flushall"),
        DDTRACE_STRING_LITERAL("flushdb"),
        DDTRACE_STRING_LITERAL("get"),
        DDTRACE_STRING_LITERAL("getbit"),
        DDTRACE_STRING_LITERAL("getkeys"),
        DDTRACE_STRING_LITERAL("getmultiple"),
        DDTRACE_STRING_LITERAL("getrange"),
        DDTRACE_STRING_LITERAL("getset"),
        DDTRACE_STRING_LITERAL("incr"),
        DDTRACE_STRING_LITERAL("incrby"),
        DDTRACE_STRING_LITERAL("incrbyfloat"),
        DDTRACE_STRING_LITERAL("keys"),
        DDTRACE_STRING_LITERAL("mget"),
        DDTRACE_STRING_LITERAL("migrate"),
        DDTRACE_STRING_LITERAL("move"),
        DDTRACE_STRING_LITERAL("mset"),
        DDTRACE_STRING_LITERAL("msetnx"),
        DDTRACE_STRING_LITERAL("object"),
        DDTRACE_STRING_LITERAL("open"),
        DDTRACE_STRING_LITERAL("pconnect"),
        DDTRACE_STRING_LITERAL("persist"),
        DDTRACE_STRING_LITERAL("pexpire"),
        DDTRACE_STRING_LITERAL("pexpireat"),
        DDTRACE_STRING_LITERAL("ping"),
        DDTRACE_STRING_LITERAL("popen"),
        DDTRACE_STRING_LITERAL("psetex"),
        DDTRACE_STRING_LITERAL("pttl"),
        DDTRACE_STRING_LITERAL("randomkey"),
        DDTRACE_STRING_LITERAL("rename"),
        DDTRACE_STRING_LITERAL("renamekey"),
        DDTRACE_STRING_LITERAL("renamenx"),
        DDTRACE_STRING_LITERAL("restore"),
        DDTRACE_STRING_LITERAL("save"),
        DDTRACE_STRING_LITERAL("scan"),
        DDTRACE_STRING_LITERAL("select"),
        DDTRACE_STRING_LITERAL("set"),
        DDTRACE_STRING_LITERAL("setbit"),
        DDTRACE_STRING_LITERAL("setex"),
        DDTRACE_STRING_LITERAL("setnx"),
        DDTRACE_STRING_LITERAL("setrange"),
        DDTRACE_STRING_LITERAL("settimeout"),
        DDTRACE_STRING_LITERAL("sort"),
        DDTRACE_STRING_LITERAL("strlen"),
        DDTRACE_STRING_LITERAL("ttl"),
        DDTRACE_STRING_LITERAL("type"),

        // Hash functions
        DDTRACE_STRING_LITERAL("hdel"),
        DDTRACE_STRING_LITERAL("hexists"),
        DDTRACE_STRING_LITERAL("hget"),
        DDTRACE_STRING_LITERAL("hgetall"),
        DDTRACE_STRING_LITERAL("hincrby"),
        DDTRACE_STRING_LITERAL("hincrbyfloat"),
        DDTRACE_STRING_LITERAL("hkeys"),
        DDTRACE_STRING_LITERAL("hlen"),
        DDTRACE_STRING_LITERAL("hmget"),
        DDTRACE_STRING_LITERAL("hmset"),
        DDTRACE_STRING_LITERAL("hscan"),
        DDTRACE_STRING_LITERAL("hset"),
        DDTRACE_STRING_LITERAL("hsetnx"),
        DDTRACE_STRING_LITERAL("hstrlen"),
        DDTRACE_STRING_LITERAL("hvals"),

        // Lists
        DDTRACE_STRING_LITERAL("blpop"),
        DDTRACE_STRING_LITERAL("brpop"),
        DDTRACE_STRING_LITERAL("brpoplpush"),
        DDTRACE_STRING_LITERAL("lget"),
        DDTRACE_STRING_LITERAL("lgetrange"),
        DDTRACE_STRING_LITERAL("lindex"),
        DDTRACE_STRING_LITERAL("linsert"),
        DDTRACE_STRING_LITERAL("listtrim"),
        DDTRACE_STRING_LITERAL("llen"),
        DDTRACE_STRING_LITERAL("lpop"),
        DDTRACE_STRING_LITERAL("lpush"),
        DDTRACE_STRING_LITERAL("lpushx"),
        DDTRACE_STRING_LITERAL("lrange"),
        DDTRACE_STRING_LITERAL("lrem"),
        DDTRACE_STRING_LITERAL("lremove"),
        DDTRACE_STRING_LITERAL("lset"),
        DDTRACE_STRING_LITERAL("lsize"),
        DDTRACE_STRING_LITERAL("ltrim"),
        DDTRACE_STRING_LITERAL("rpop"),
        DDTRACE_STRING_LITERAL("rpoplpush"),
        DDTRACE_STRING_LITERAL("rpush"),
        DDTRACE_STRING_LITERAL("rpushx"),

        DDTRACE_STRING_LITERAL("rawcommand"),
    };
    // clang-format on

    ddtrace_string phpredis = DDTRACE_STRING_LITERAL("redis");
    size_t methods_len = sizeof methods / sizeof methods[0];
    ddtrace_replace_internal_methods(phpredis, methods_len, methods);
}
