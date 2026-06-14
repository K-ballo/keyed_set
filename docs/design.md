<!--
Eggs.KeyedSet

Copyright Agustin K-ballo Berge, Fusion Fenix 2026

Distributed under the Boost Software License, Version 1.0. (See
accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
-->

# Design

## Key as a non-type template parameter

The member that serves as the key is given as a pointer-to-member
(`auto Key`), not as a runtime projection function. This makes the key
member a compile-time constant rather than an object stored inside the
container. The container type encodes the key member, so two containers
that differ only in which field they key on are distinct types — the
compiler rejects accidental mixing.

Encoding the key in the type also means the projecting comparator has
zero runtime overhead compared to a hand-written `std::set` with a
custom comparator.

## Const-only iterators

Iterators into a `keyed_set` are always `const`. The C++26 AssociativeContainer
requirements ([associative.reqmts.general]/p6) forbid mutating an element
through an iterator because the element's position in the tree depends on
its key member. Allowing a mutation that changes the key would silently
corrupt the container's ordering invariant.

Making `iterator` an alias for `const_iterator` enforces this at the type
level, matching the behavior of `std::set`. Only `const`-qualified accessor
overloads are provided; a separate non-`const` overload would be a
redefinition.

## Projecting comparator

`std::set` requires a comparator that operates on `Value` objects.
`keyed_set` wraps the user-supplied `Compare` in an internal `comparator`
adaptor that projects each operand by `v.*Key` before invoking `Compare`.

The adaptor always defines `is_transparent` because `std::set` checks for
that tag to activate its heterogeneous tree operations. Those operations are
needed whenever the lookup key type differs from `Value`, which is always
true here. The generic `K` overloads — which allow lookup by types other
than `key_type` — are enabled only when `Compare::is_transparent` is
defined, matching the standard library opt-in convention: a non-transparent
comparator such as `std::less<int>` is not expected to handle arbitrary `K`.

## Backed by `std::set`

`keyed_set` is a thin wrapper around `std::set<Value, _comparator, Allocator>`.
Tree balancing, iterator stability, node-handle support, and allocator
propagation all come from `std::set` without reimplementation.

## Standard conformance

`keyed_set` satisfies the C++26 AssociativeContainer requirements
([associative.reqmts.general]), including all required nested types,
constructors, modifiers, and lookup operations. The transparent overloads
for lookup, erase, and extract follow the same opt-in convention as
`std::set` and `std::map`: they participate in overload resolution only
when `Compare::is_transparent` is defined.

## Three-way comparison

`operator<=>` cannot delegate directly to `std::set::operator<=>` because
that operator uses `__synth3way`, which requires `Value::operator<`. A
`Value` that provides only `operator<=>` would not compile. `keyed_set`
instead calls `std::lexicographical_compare_three_way` with an explicit
`Value::operator<=>` comparator, which works for any `Value` that supports
three-way comparison.
