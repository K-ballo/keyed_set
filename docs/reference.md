<!--
Eggs.KeyedSet

Copyright Agustin K-ballo Berge, Fusion Fenix 2026

Distributed under the Boost Software License, Version 1.0. (See
accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-->

# Reference

## Header

### `<eggs/keyed_set.hpp>`

A single C++ header providing `eggs::keyed_set` and `eggs::erase_if`.
Has a standard include guard. Requires C++23.

---

## `eggs::keyed_set`

```cpp
namespace eggs {

template <
    typename Value,
    auto     Key,
    typename Compare   = std::less</*key_type*/>,
    typename Allocator = std::allocator<Value>
>
class keyed_set;

} // namespace eggs
```

An ordered associative container storing unique `Value` objects ordered
by the member designated by `Key`. Satisfies the C++26 AssociativeContainer
requirements ([associative.reqmts.general]).

### Template parameters

| Parameter | Description |
|-----------|-------------|
| `Value` | Element type stored in the container |
| `Key` | Pointer-to-member designating the key field (e.g. `&Employee::id`) |
| `Compare` | Strict-weak-ordering predicate on `key_type`. Defaults to `std::less<key_type>` |
| `Allocator` | Allocator for `Value`. Defaults to `std::allocator<Value>` |

### Member types

| Name | Description |
|------|-------------|
| `key_type` | Type of the key member (e.g. `int` for `&Employee::id`) |
| `value_type` | `Value` |
| `key_compare` | `Compare` |
| `value_compare` | Internal projecting comparator; orders `Value` objects by their key member |
| `allocator_type` | `Allocator` |
| `pointer` | `allocator_traits<Allocator>::pointer` |
| `const_pointer` | `allocator_traits<Allocator>::const_pointer` |
| `reference` | `value_type&` |
| `const_reference` | `value_type const&` |
| `size_type` | Unsigned integer type |
| `difference_type` | Signed integer type |
| `iterator` | Constant bidirectional iterator (same type as `const_iterator`) |
| `const_iterator` | Constant bidirectional iterator |
| `reverse_iterator` | Constant reverse bidirectional iterator |
| `const_reverse_iterator` | Constant reverse bidirectional iterator |
| `node_type` | Node-handle type |
| `insert_return_type` | Return type of node-handle `insert` |

### Constructors

```cpp
keyed_set();
explicit keyed_set(key_compare const& c, allocator_type const& a = {});
explicit keyed_set(allocator_type const& a);

template <typename InputIt>
keyed_set(InputIt first, InputIt last,
          key_compare const& c = {}, allocator_type const& a = {});

template <typename InputIt>
keyed_set(InputIt first, InputIt last, allocator_type const& a);

// Requires __cpp_lib_ranges_to_container
template <std::ranges::input_range R>
keyed_set(std::from_range_t, R&& rg,
          key_compare const& c = {}, allocator_type const& a = {});

template <std::ranges::input_range R>
keyed_set(std::from_range_t, R&& rg, allocator_type const& a);

keyed_set(std::initializer_list<value_type> il,
          key_compare const& c = {}, allocator_type const& a = {});
keyed_set(std::initializer_list<value_type> il, allocator_type const& a);

keyed_set(keyed_set const&);
keyed_set(keyed_set&&) noexcept;
keyed_set(keyed_set const&, allocator_type const& a);
keyed_set(keyed_set&&,      allocator_type const& a);
```

### Assignment

```cpp
keyed_set& operator=(keyed_set const&);
keyed_set& operator=(keyed_set&&) noexcept;
keyed_set& operator=(std::initializer_list<value_type>);
```

### Allocator

```cpp
allocator_type get_allocator() const noexcept;
```

### Iterators

```cpp
const_iterator begin()  const noexcept;
const_iterator end()    const noexcept;
const_iterator cbegin() const noexcept;
const_iterator cend()   const noexcept;

const_reverse_iterator rbegin()  const noexcept;
const_reverse_iterator rend()    const noexcept;
const_reverse_iterator crbegin() const noexcept;
const_reverse_iterator crend()   const noexcept;
```

All iterators are `const`. The key member of a stored element cannot be
modified through an iterator.

### Capacity

```cpp
[[nodiscard]] bool empty()    const noexcept;
size_type          size()     const noexcept;
size_type          max_size() const noexcept;
```

### Modifiers

**Emplace / insert**

```cpp
template <typename... Args>
std::pair<iterator, bool> emplace(Args&&... args);

template <typename... Args>
iterator emplace_hint(const_iterator hint, Args&&... args);

std::pair<iterator, bool> insert(value_type const& v);
std::pair<iterator, bool> insert(value_type&& v);
iterator insert(const_iterator hint, value_type const& v);
iterator insert(const_iterator hint, value_type&& v);

template <typename InputIt>
void insert(InputIt first, InputIt last);

void insert(std::initializer_list<value_type> il);

insert_return_type insert(node_type&& nh);
iterator           insert(const_iterator hint, node_type&& nh);

// Requires __cpp_lib_ranges_to_container
template <std::ranges::input_range R>
void insert_range(R&& rg);
```

`insert` and `emplace` return `{iterator, true}` when the element was
inserted, or `{iterator-to-existing, false}` when an equivalent key
already existed.

**Extract**

```cpp
node_type extract(const_iterator q);
node_type extract(key_type const& k);

// Transparent — requires Compare::is_transparent
//               and __cpp_lib_associative_heterogeneous_erasure
template <typename K>
    requires(!std::is_convertible_v<K&&, iterator> &&
             !std::is_convertible_v<K&&, const_iterator>)
node_type extract(K&& kx);
```

**Erase**

```cpp
size_type erase(key_type const& k);        // returns 0 or 1
iterator  erase(const_iterator q);
iterator  erase(const_iterator q1, const_iterator q2);

// Transparent — requires Compare::is_transparent
//               and __cpp_lib_associative_heterogeneous_erasure
template <typename K>
    requires(!std::is_convertible_v<K&&, iterator> &&
             !std::is_convertible_v<K&&, const_iterator>)
size_type erase(K&& kx);
```

**Other**

```cpp
void clear() noexcept;

void merge(keyed_set& other);
void merge(keyed_set&& other);
```

`merge` moves elements from `other` into this container. Elements whose
key already exists in the destination are left in `other`.

### Observers

```cpp
key_compare   key_comp()   const;
value_compare value_comp() const;
```

`key_comp()` returns the `Compare` object passed at construction.
`value_comp()` returns the internal projecting comparator that orders
`Value` objects by their key member.

### Lookup

```cpp
const_iterator find(key_type const& k) const;
size_type      count(key_type const& k) const;
bool           contains(key_type const& k) const;

const_iterator lower_bound(key_type const& k) const;
const_iterator upper_bound(key_type const& k) const;
std::pair<const_iterator, const_iterator> equal_range(key_type const& k) const;
```

Transparent overloads — require `Compare::is_transparent`:

```cpp
template <typename K> const_iterator find(K const& k) const;
template <typename K> size_type      count(K const& k) const;
template <typename K> bool           contains(K const& k) const;
template <typename K> const_iterator lower_bound(K const& k) const;
template <typename K> const_iterator upper_bound(K const& k) const;
template <typename K>
std::pair<const_iterator, const_iterator> equal_range(K const& k) const;
```

### Swap

```cpp
void swap(keyed_set& other) noexcept;
friend void swap(keyed_set& a, keyed_set& b) noexcept;
```

### Comparison

```cpp
friend bool operator==(keyed_set const& a, keyed_set const& b);
friend auto operator<=>(keyed_set const& a, keyed_set const& b);
```

`operator<=>` performs lexicographic three-way comparison using
`value_type::operator<=>`. It synthesises all six relational operators.

---

## `eggs::erase_if`

```cpp
namespace eggs {

template <typename Value, auto Key, typename Compare, typename Allocator,
          typename Predicate>
typename keyed_set<Value, Key, Compare, Allocator>::size_type
erase_if(keyed_set<Value, Key, Compare, Allocator>& c, Predicate pred);

} // namespace eggs
```

Erases all elements from `c` for which `pred(*it)` returns `true`.
Returns the number of elements erased.
