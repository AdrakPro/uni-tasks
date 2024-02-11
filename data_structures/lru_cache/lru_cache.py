# Task: Implement an LRU cache
# Thinking process:
# What is LRU? It's Least Recently Used. Criteria of eviction from cache:
# 1. Expiration time -> if time expired, evict
# 2. Priority from 0, lower priority will first get evicted then higher
# 3. Least recently used -> evict item which was least recently used
# Naive, minimal plausible solution with future options to optimize.
# Implement Cache and Item:
import bisect
import functools
import math
import operator
import random
import time
from collections import OrderedDict
from typing import NamedTuple


class Item(NamedTuple):
    key: str
    value: object
    expires: int
    priority: int


class Cache:
    def __init__(self, max_size: int, cache_time=time.monotonic):
        self.max_size = max_size
        self.time = cache_time
        # Dict provides average O(1) search/insert/delete
        self.cache = {}
        self.expires_buckets = {}
        self.expires_order = PriorityQueue()
        # OrderedDict -> searching from functools.lru_cache() source code (doubly linked list)
        self.priority_buckets = {}
        self.priority_order = PriorityQueue()

    # Functions needed in cache:
    # get(key: String)
    # set(key: String, value: Object, max_age: Int, priority=0)
    # evict(now: time)
    def get(self, key: str):
        # Check if the key is in cache and not expired
        item = self.cache.get(key)

        if not item:
            return None

        if self.time() >= item.expires:
            return None

        # If data is used move to the end to avoid evict
        self.priority_buckets[item.priority].move_to_end(key)

        return item.value

    def set(self, key: str, value: object, *, max_age=10, priority=0):
        now = self.time()

        # If the same element is in cache remove and insert again
        # Tuples (expires, key) compare lexicographically, it will be like comparing by expires alone, but with key along
        if key in self.cache:
            self.delete(key)
        # Evict if the max cache size is exceeded
        elif len(self.cache) >= self.max_size:
            self.evict(now)

        expires = log_bucket(now, max_age, shift=7)
        priority = log_bucket(0, priority + 1, shift=7)

        self.cache[key] = Item(key, value, expires, priority)

        # If bucket is not existing, create new one. Insert empty data
        expires_bucket = self.expires_buckets.get(expires)
        if not expires_bucket:
            expires_bucket = self.expires_buckets[expires] = set()
            self.expires_order.insert(expires)
        expires_bucket.add(key)

        priority_bucket = self.priority_buckets.get(priority)
        if not priority_bucket:
            priority_bucket = self.priority_buckets[priority] = OrderedDict()
            self.priority_order.insert(priority)
        priority_bucket[key] = None

    # First criteria of eviction -> Expiration time
    # Second criteria of eviction -> Priority
    # Third criteria of eviction -> LRU
    def evict(self, now):
        if not self.cache:
            return

        initial_size = len(self.cache)

        while self.cache:
            expires = self.expires_order.peek()

            if expires > now:
                break

            expires_bucket = self.expires_buckets[expires]
            for key in list(expires_bucket):
                self.delete(key)

        # If none expired, remove with the lowest priority
        if len(self.cache) == initial_size:
            priority = self.priority_order.peek()
            priority_bucket = self.priority_buckets.get(priority)
            key = next(iter(priority_bucket))
            self.delete(key)

    def delete(self, key):
        # * -> unpack tuple and return two last values (priority, expires)
        *_, expires, priority = self.cache.pop(key)

        expires_bucket = self.expires_buckets[expires]
        expires_bucket.remove(key)
        if not expires_bucket:
            del self.expires_buckets[expires]
            self.expires_order.remove(expires)

        # Don't get rid of empty buckets
        priority_bucket = self.priority_buckets[priority]
        del priority_bucket[key]

        # Cleanup
        if not priority_bucket:
            del self.priority_buckets[priority]
            self.priority_order.remove(priority)


# Need a data structure which will efficiently remove the smallest element
# https://en.wikipedia.org/wiki/Priority_queue
class PriorityQueue:
    def __init__(self):
        self.data = []

    # O(1)
    def is_empty(self):
        return len(self.data) == 0

    # Handle the reverse order (0 -> -1)
    def peek(self):
        return self.data[-1]

    def pop(self):
        return self.data.pop()

    def remove(self, item):
        i = bisect.bisect_left(self.data, -item, key=operator.neg)
        if i != len(self.data) and self.data[i] == item:
            del self.data[i]
            return
        raise ValueError

    # operator.neg() to keep the list reversed
    def insert(self, item):
        bisect.insort(self.data, item, key=operator.neg)


# Possible optimizations:
# 1. heapq -> priority queue algorithm, using (priority, item) tuples, mark the entry as removed and add a new entry with the revised priority
# 2. bisect -> provides support for maintaining lists in sorted order without having to sort the list after each insertion
# 3. pop() optimization -> reverse the order, we'd pop() from the end
# 4. Binary search tree
# 5. Sorted containers -> pure python library that is faster than BST.
# Comparison with trees: fewer memory allocations, better cache locality, lower memory overhead, and faster iteration
# Limitations of our cache:
# Do I need more than 10k priorities? No
# Do I need more than 10k expiry times? Maybe? 1 second granularity can represent only up to 2.7 hours. 10 seconds to 27 hours
# Does it matter if items expire in 2010 seconds instead of 2001. No. But need a way to round small values with higher granularity than big ones
# The solution is to use logarithmic time
# 	        sort	heapq	    bisect	bisect <10k	BST
# push()	O(n)	O(log n)	O(n)	O(log n)	O(log n)
# peek()	O(1)	O(1)	    O(1)	O(1)	    O(log n)
# pop()	    O(1)*	O(log n)	O(1)*	O(1)*	    O(log n)
# remove()	O(n)	O(log n)	O(n)	O(log n)	O(log n)


def log_bucket(now, max_age, shift=0):
    next_power = 2 ** max(0, math.ceil(math.log2(max_age) - shift))
    expires = now + max_age
    bucket = math.ceil(expires / next_power) * next_power
    return bucket


def error(now, max_age, *args):
    """log_bucket() error."""
    bucket = log_bucket(now, max_age, *args)
    return (bucket - now) / max_age - 1


def max_error(now, max_maxage, *args):
    """Worst log_bucket() error for all max_ages up to max_maxage."""
    return max(error(now, max_age, *args) for max_age in range(1, max_maxage))


def max_error_random(n, *args):
    """Worst log_bucket() error for random inputs, out of n tries."""
    max_now = int(time.time()) * 2
    max_maxage = 3600 * 24 * 31
    rand = functools.partial(random.randint, 1)
    return max(error(rand(max_now), rand(max_maxage), *args) for _ in range(n))


def max_buckets(max_maxage, *args):
    """Number of buckets to cover all max_ages up to max_maxage."""
    now = time.time()
    buckets = {log_bucket(now, max_age, *args) for max_age in range(1, max_maxage)}
    return len(buckets)
