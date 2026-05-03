// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_KEYED_SET_HPP
#define EGGS_KEYED_SET_HPP

#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <ranges>
#include <set>
#include <type_traits>
#include <utility>

namespace eggs
{
    ///////////////////////////////////////////////////////////////////////////
    //! \namespace eggs::keyed_set_detail
    //!
    //! Implementation details; not part of the public interface.
    namespace keyed_set_detail
    {
        ///////////////////////////////////////////////////////////////////////
        //! \struct member_key_type
        //!
        //! Extracts the value type of a pointer-to-member via partial
        //! specialisation. This indirection is required because the NTTP
        //! syntax `auto Value::*Key` is not yet supported by all C++20/23
        //! compilers (notably GCC < 14); accepting `auto Key` and
        //! specialising on `T Value::*` is universally supported.
        template <typename Value, auto Key>
        struct member_key_type;

        template <typename Value, typename T, T Value::*Key>
        struct member_key_type<Value, Key>
        {
            using type = std::remove_cvref_t<T>;
        };

        ///////////////////////////////////////////////////////////////////////
        //! \struct comparator
        //!
        //! Projecting comparator adaptor. Wraps a user-supplied `Compare`
        //! and applies it to `v.*Key` rather than directly to `Value`.
        template <typename Value, auto Key, typename Compare>
        struct comparator : Compare  // EBO
        {
            using key_type = typename member_key_type<Value, Key>::type;

            // Always defined so that std::set activates its heterogeneous
            // tree operations (_M_find_tr etc.), which we need because our
            // key_type differs from Value. The actual type does not matter;
            // void is the conventional sentinel.
            using is_transparent = void;

            comparator() = default;

            explicit comparator(Compare const& c)
                : Compare(c)
            {}

            explicit comparator(Compare&& c)
                : Compare(std::move(c))
            {}

            Compare const& key_comp() const noexcept
            {
                return static_cast<Compare const&>(*this);
            }

            // Value vs Value
            constexpr bool operator()(Value const& a, Value const& b) const
                noexcept(noexcept(key_comp()(a.*Key, b.*Key)))
            { return key_comp()(a.*Key, b.*Key); }

            // key_type vs Value  (always available)
            constexpr bool operator()(key_type const& k, Value const& b) const
                noexcept(noexcept(key_comp()(k, b.*Key)))
            { return key_comp()(k, b.*Key); }

            // Value vs key_type  (always available)
            constexpr bool operator()(Value const& a, key_type const& k) const
                noexcept(noexcept(key_comp()(a.*Key, k)))
            { return key_comp()(a.*Key, k); }

            // Transparent heterogeneous overloads — only available when
            // Compare::is_transparent is defined (SFINAE via the using above).
            template <typename K>
            constexpr bool operator()(K const& k, Value const& b) const
                noexcept(noexcept(key_comp()(k, b.*Key)))
            { return key_comp()(k, b.*Key); }

            template <typename K>
            constexpr bool operator()(Value const& a, K const& k) const
                noexcept(noexcept(key_comp()(a.*Key, k)))
            { return key_comp()(a.*Key, k); }

            // key_type vs key_type  — forwarded directly to Compare, so
            // std::set can compare extracted keys during rebalancing.
            constexpr bool operator()(key_type const& a, key_type const& b) const
                noexcept(noexcept(key_comp()(a, b)))
            { return key_comp()(a, b); }
        };

    } // namespace keyed_set_detail

    ///////////////////////////////////////////////////////////////////////////
    //! \class keyed_set
    //!
    //! An ordered associative container satisfying the C++26 AssociativeContainer
    //! requirements ([associative.reqmts]) that stores unique `Value` objects
    //! indexed by a member designated by the non-type template parameter `Key`.
    //!
    //! \tparam Value  The element type. Must be *Cpp17Erasable* from the
    //!                container and satisfy the requirements of the
    //!                underlying `std::set`.
    //! \tparam Key     A pointer-to-member of `Value` whose pointee type
    //!                 becomes `key_type`.
    //! \tparam Compare    A strict-weak-ordering binary predicate on `key_type`.
    //!                    Defaults to `std::less<key_type>`.
    //! \tparam Allocator  Allocator for `Value`. Defaults to `std::allocator<Value>`.
    //!
    //! ### Example
    //! \code
    //! struct Employee { int id; std::string name; };
    //!
    //! eggs::keyed_set<Employee, &Employee::id> by_id;
    //! by_id.insert({42, "Alice"});
    //! by_id.insert({7,  "Bob"});
    //!
    //! auto it = by_id.find(42);          // heterogeneous lookup by int
    //! assert(it->name == "Alice");
    //! \endcode
    template <typename Value, auto Key,
              typename Compare   = std::less<typename keyed_set_detail::member_key_type<Value, Key>::type>,
              typename Allocator = std::allocator<Value>>
    class keyed_set
    {
        using _comparator = keyed_set_detail::comparator<Value, Key, Compare>;
        using _set_type   = std::set<Value, _comparator, Allocator>;

    public:
        // ── Required type members [associative.reqmts.general] ───────────────

        using key_type        = typename _comparator::key_type;
        using value_type      = Value;
        using key_compare     = _comparator;

        //! For set-like containers where `value_type` is the same as
        //! `key_type`, `value_compare` is the same type as `key_compare`
        //! ([associative.reqmts.general]/p16).
        using value_compare   = _comparator;

        //! The underlying comparison object supplied by the user.
        using compare_type    = Compare;

        using allocator_type  = typename _set_type::allocator_type;
        using pointer         = typename _set_type::pointer;
        using const_pointer   = typename _set_type::const_pointer;
        using reference       = typename _set_type::reference;
        using const_reference = typename _set_type::const_reference;
        using size_type       = typename _set_type::size_type;
        using difference_type = typename _set_type::difference_type;

        //! Iterators are constant because the key member of a stored element
        //! must not be mutated through an iterator
        //! ([associative.reqmts.general]/p6).
        //! Because `iterator` and `const_iterator` are the same type, only
        //! `const`-qualified overloads are provided; a separate non-`const`
        //! overload would be a redefinition.
        using iterator        = typename _set_type::const_iterator;
        using const_iterator  = typename _set_type::const_iterator;

        using reverse_iterator       = typename _set_type::const_reverse_iterator;
        using const_reverse_iterator = typename _set_type::const_reverse_iterator;

        //! Node-handle types required since C++17 ([container.node]).
        using node_type          = typename _set_type::node_type;
        using insert_return_type = typename _set_type::insert_return_type;

        // ── Constructors ─────────────────────────────────────────────────────

        //! Default constructor. Constructs an empty container using
        //! `key_compare()` as comparison object ([associative.reqmts.general]/p20-22).
        keyed_set()
            : keyed_set(key_compare{})
        {}

        //! Constructs an empty container using `c` as comparison object
        //! ([associative.reqmts.general]/p18-19).
        explicit keyed_set(key_compare const& c,
                           allocator_type const& a = allocator_type{})
            : set_(c, a)
        {}

        //! Constructs an empty container using `c` as the underlying
        //! `Compare` object (convenience overload).
        explicit keyed_set(Compare const& c,
                           allocator_type const& a = allocator_type{})
            : set_(key_compare{c}, a)
        {}

        //! Constructs an empty container using the given allocator.
        explicit keyed_set(allocator_type const& a)
            : set_(a)
        {}

        //! Range constructor. Inserts elements from `[first, last)`
        //! ([associative.reqmts.general]/p23-25).
        template <typename InputIt>
        keyed_set(InputIt first, InputIt last,
                  key_compare const& c = key_compare{},
                  allocator_type const& a = allocator_type{})
            : set_(first, last, c, a)
        {}

        //! Range constructor with allocator-only comparison.
        template <typename InputIt>
        keyed_set(InputIt first, InputIt last,
                  allocator_type const& a)
            : set_(first, last, a)
        {}

#ifdef __cpp_lib_ranges_to_container
        //! Range constructor from a `container-compatible-range`
        //! ([associative.reqmts.general]/p29-31).
        //!
        //! \note We do not delegate to `std::set`'s own `from_range`
        //! constructor because it may not be available even when
        //! `std::from_range_t` is (e.g. libstdc++ shipped with GCC 14).
        //! Instead we construct via the iterator-range path, which is
        //! universally available.
        template <std::ranges::input_range R>
            requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        keyed_set(std::from_range_t, R&& rg,
                  key_compare const& c = key_compare{},
                  allocator_type const& a = allocator_type{})
            : set_(std::ranges::begin(rg), std::ranges::end(rg), c, a)
        {}

        //! Range constructor from a `container-compatible-range` with allocator
        //! ([associative.reqmts.general]/p32-34).
        template <std::ranges::input_range R>
            requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        keyed_set(std::from_range_t, R&& rg, allocator_type const& a)
            : set_(std::ranges::begin(rg), std::ranges::end(rg), a)
        {}
#endif

        //! Initializer-list constructor
        //! ([associative.reqmts.general]/p35).
        keyed_set(std::initializer_list<value_type> il,
                  key_compare const& c = key_compare{},
                  allocator_type const& a = allocator_type{})
            : set_(il, c, a)
        {}

        //! Initializer-list constructor with allocator.
        keyed_set(std::initializer_list<value_type> il,
                  allocator_type const& a)
            : set_(il, a)
        {}

        keyed_set(keyed_set const&) = default;
        keyed_set(keyed_set&&) noexcept = default;

        keyed_set(keyed_set const& other, allocator_type const& a)
            : set_(other.set_, a)
        {}

        keyed_set(keyed_set&& other, allocator_type const& a)
            : set_(std::move(other.set_), a)
        {}

        ~keyed_set() = default;

        // ── Assignment ───────────────────────────────────────────────────────

        keyed_set& operator=(keyed_set const&) = default;
        keyed_set& operator=(keyed_set&&) noexcept = default;

        //! Assigns from an initializer list
        //! ([associative.reqmts.general]/p37-40).
        keyed_set& operator=(std::initializer_list<value_type> il)
        {
            set_ = il;
            return *this;
        }

        // ── Allocator ────────────────────────────────────────────────────────

        allocator_type get_allocator() const noexcept
        {
            return set_.get_allocator();
        }

        // ── Iterators ────────────────────────────────────────────────────────

        const_iterator  begin()  const noexcept { return set_.begin();  }
        const_iterator  end()    const noexcept { return set_.end();    }
        const_iterator  cbegin() const noexcept { return set_.cbegin(); }
        const_iterator  cend()   const noexcept { return set_.cend();   }

        const_reverse_iterator  rbegin()  const noexcept { return set_.rbegin();  }
        const_reverse_iterator  rend()    const noexcept { return set_.rend();    }
        const_reverse_iterator  crbegin() const noexcept { return set_.crbegin(); }
        const_reverse_iterator  crend()   const noexcept { return set_.crend();   }

        // ── Capacity ─────────────────────────────────────────────────────────

        [[nodiscard]] bool empty()    const noexcept { return set_.empty();    }
        size_type          size()     const noexcept { return set_.size();     }
        size_type          max_size() const noexcept { return set_.max_size(); }

        // ── Modifiers ────────────────────────────────────────────────────────

        //! Constructs a new element in-place if no equivalent key exists
        //! ([associative.reqmts.general]/p47-51).
        template <typename... Args>
        std::pair<iterator, bool> emplace(Args&&... args)
        {
            return set_.emplace(std::forward<Args>(args)...);
        }

        //! Constructs a new element in-place near `hint`
        //! ([associative.reqmts.general]/p57-60).
        template <typename... Args>
        iterator emplace_hint(const_iterator hint, Args&&... args)
        {
            return set_.emplace_hint(hint, std::forward<Args>(args)...);
        }

        //! Inserts `v` if no equivalent key exists; returns `{pos, inserted}`
        //! ([associative.reqmts.general]/p61-65).
        std::pair<iterator, bool> insert(value_type const& v)
        {
            return set_.insert(v);
        }

        std::pair<iterator, bool> insert(value_type&& v)
        {
            return set_.insert(std::move(v));
        }

        //! Inserts `v` near `hint`
        //! ([associative.reqmts.general]/p70-74).
        iterator insert(const_iterator hint, value_type const& v)
        {
            return set_.insert(hint, v);
        }

        iterator insert(const_iterator hint, value_type&& v)
        {
            return set_.insert(hint, std::move(v));
        }

        //! Inserts elements from the range `[first, last)`
        //! ([associative.reqmts.general]/p75-78).
        template <typename InputIt>
        void insert(InputIt first, InputIt last)
        {
            set_.insert(first, last);
        }

#ifdef __cpp_lib_ranges_to_container
        //! Inserts elements from a `container-compatible-range`
        //! ([associative.reqmts.general]/p79-82).
        //!
        //! \note Delegates to the iterator-range `insert` rather than
        //! `std::set::insert_range` for the same reason as the `from_range`
        //! constructor: the set method may be unavailable even when
        //! `std::from_range_t` is defined.
        template <std::ranges::input_range R>
            requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        void insert_range(R&& rg)
        {
            set_.insert(std::ranges::begin(rg), std::ranges::end(rg));
        }
#endif

        //! Inserts from an initializer list
        //! ([associative.reqmts.general]/p83).
        void insert(std::initializer_list<value_type> il)
        {
            set_.insert(il);
        }

        //! Inserts a node-handle
        //! ([associative.reqmts.general]/p84-88).
        insert_return_type insert(node_type&& nh)
        {
            return set_.insert(std::move(nh));
        }

        //! Inserts a node-handle near `hint`
        //! ([associative.reqmts.general]/p94-99).
        iterator insert(const_iterator hint, node_type&& nh)
        {
            return set_.insert(hint, std::move(nh));
        }

        //! Extracts the element pointed to by `q`
        //! ([associative.reqmts.general]/p108-111).
        node_type extract(const_iterator q)
        {
            return set_.extract(q);
        }

        //! Extracts the element with key equivalent to `k`
        //! ([associative.reqmts.general]/p100-103).
        //!
        //! \note `std::set::key_type` is `Value`, not our `key_type`, so we
        //! find by key and extract by iterator, which is universally available.
        node_type extract(key_type const& k)
        {
            auto it = set_.find(k);
            if (it == set_.end())
                return {};
            return set_.extract(it);
        }

#ifdef __cpp_lib_associative_heterogeneous_erasure
        //! Transparent extract: extracts without constructing a `key_type`
        //! ([associative.reqmts.general]/p104-107).
        //!
        //! \note Does not participate in overload resolution if `K` is
        //! convertible to `iterator` or `const_iterator`, per the standard.
        //! Requires P2077R3 (libstdc++ >= 14).
        template <typename K>
            requires (requires { typename Compare::is_transparent; } &&
                      !std::is_convertible_v<K&&, iterator> &&
                      !std::is_convertible_v<K&&, const_iterator>)
        node_type extract(K&& kx)
        {
            return set_.extract(std::forward<K>(kx));
        }
#endif

        //! Erases all elements with key equivalent to `k`; returns count
        //! ([associative.reqmts.general]/p118-121).
        //!
        //! \note Same rationale as `extract(key_type const&)`: find by key,
        //! then erase by iterator.
        size_type erase(key_type const& k)
        {
            auto it = set_.find(k);
            if (it == set_.end())
                return 0;
            set_.erase(it);
            return 1;
        }

#ifdef __cpp_lib_associative_heterogeneous_erasure
        //! Transparent erase
        //! ([associative.reqmts.general]/p122-125).
        //! Requires P2077R3 (libstdc++ >= 14).
        template <typename K>
            requires (requires { typename Compare::is_transparent; } &&
                      !std::is_convertible_v<K&&, iterator> &&
                      !std::is_convertible_v<K&&, const_iterator>)
        size_type erase(K&& kx)
        {
            return set_.erase(std::forward<K>(kx));
        }
#endif

        //! Erases the element pointed to by `q`
        //! ([associative.reqmts.general]/p126-133).
        //! (`iterator` and `const_iterator` are the same type, so a single
        //! overload covers both the mutable and constant iterator cases.)
        iterator erase(const_iterator q)
        {
            return set_.erase(q);
        }

        //! Erases elements in `[q1, q2)`
        //! ([associative.reqmts.general]/p134-137).
        iterator erase(const_iterator q1, const_iterator q2)
        {
            return set_.erase(q1, q2);
        }

        //! Clears the container
        //! ([associative.reqmts.general]/p138-140).
        void clear() noexcept
        {
            set_.clear();
        }

        //! Merges elements from `a2` into this container
        //! ([associative.reqmts.general]/p112-117).
        void merge(keyed_set& other)
        {
            set_.merge(other.set_);
        }

        void merge(keyed_set&& other)
        {
            set_.merge(std::move(other.set_));
        }

        // ── Observers ────────────────────────────────────────────────────────

        key_compare key_comp() const
        {
            return set_.key_comp();
        }

        //! For containers where `value_type == key_type`,
        //! `value_compare` is the same as `key_compare`
        //! ([associative.reqmts.general]/p16).
        value_compare value_comp() const
        {
            return set_.key_comp();
        }

        //! Returns the underlying `Compare` object.
        compare_type compare() const
        {
            return set_.key_comp().key_comp();
        }

        // ── Lookup ───────────────────────────────────────────────────────────

        const_iterator find(key_type const& k) const
        {
            return set_.find(k);
        }

        //! Transparent find ([associative.reqmts.general]/p144-146).
        template <typename K>
        const_iterator find(K const& k) const
        {
            return set_.find(k);
        }

        size_type count(key_type const& k) const
        {
            return set_.count(k);
        }

        //! Transparent count ([associative.reqmts.general]/p150-152).
        template <typename K>
        size_type count(K const& k) const
        {
            return set_.count(k);
        }

        bool contains(key_type const& k) const
        {
            return set_.contains(k);
        }

        //! Transparent contains ([associative.reqmts.general]/p155-156).
        template <typename K>
            requires requires { typename Compare::is_transparent; }
        bool contains(K const& k) const
        {
            return set_.contains(k);
        }

        const_iterator lower_bound(key_type const& k) const
        {
            return set_.lower_bound(k);
        }

        //! Transparent lower_bound ([associative.reqmts.general]/p160-162).
        template <typename K>
            requires requires { typename Compare::is_transparent; }
        const_iterator lower_bound(K const& k) const
        {
            return set_.lower_bound(k);
        }

        const_iterator upper_bound(key_type const& k) const
        {
            return set_.upper_bound(k);
        }

        //! Transparent upper_bound ([associative.reqmts.general]/p166-168).
        template <typename K>
            requires requires { typename Compare::is_transparent; }
        const_iterator upper_bound(K const& k) const
        {
            return set_.upper_bound(k);
        }

        std::pair<const_iterator, const_iterator> equal_range(key_type const& k) const
        {
            return set_.equal_range(k);
        }

        //! Transparent equal_range.
        template <typename K>
            requires requires { typename Compare::is_transparent; }
        std::pair<const_iterator, const_iterator> equal_range(K const& k) const
        {
            return set_.equal_range(k);
        }

        // ── Swap ─────────────────────────────────────────────────────────────

        void swap(keyed_set& other) noexcept
        {
            set_.swap(other.set_);
        }

        // ── Comparison ───────────────────────────────────────────────────────

        friend bool operator==(keyed_set const& a, keyed_set const& b)
        {
            if (a.size() != b.size())
                return false;
            return std::equal(a.begin(), a.end(), b.begin(),
                [](value_type const& x, value_type const& y)
                { return x.*Key == y.*Key; });
        }

        //! Lexicographic three-way comparison over the key projection.
        //! Synthesises all six relational operators (C++20).
        //!
        //! \note We cannot use `set_ <=> set_` directly: `std::set::operator<=>`
        //! uses `__synth3way` which requires `Value::operator<`, but `Value`
        //! may only provide `operator<=>`. We compare key members directly,
        //! which only requires `key_type::operator<=>`.
        friend auto operator<=>(keyed_set const& a, keyed_set const& b)
        {
            return std::lexicographical_compare_three_way(
                a.begin(), a.end(),
                b.begin(), b.end(),
                [](value_type const& x, value_type const& y)
                { return x.*Key <=> y.*Key; });
        }

        // ── Non-member swap (ADL) ─────────────────────────────────────────────

        friend void swap(keyed_set& a, keyed_set& b) noexcept
        {
            a.swap(b);
        }

    private:
        _set_type set_;
    };

} // namespace eggs

#endif // EGGS_KEYED_SET_HPP
