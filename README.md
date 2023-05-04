# orderbook

Orderbook with limit and iceberg orders functionality.

## Building the Release version with Cmake

```bash
cmake --preset release
cmake --build build
```

You can find the binary file at `build/orderbook`

## Doc
I use the approach that storing limit prices in a self-balancing tree (Red-Black tree in this case). Also each node of limit price contains a linked list of orders.

With this structure, we can easily implement these key operations with good performance:

* M: Number of limit prices
* N: Number of orders
* Add - O(logM) for the first order of the limit which is insignificant because M << N , O(1) otherwises
* Matching - O(1)
* Get best bid/ask offers - O(1)

We can also implement the following operations as wished with an extra hash map that maps the order's id with its position in the linked list:

* Update - O(1)
* Cancel - O(1)
