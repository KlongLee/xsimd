/***************************************************************************
 * Copyright (c) Johan Mabille, Sylvain Corlay, Wolf Vollprecht and         *
 * Martin Renou                                                             *
 * Copyright (c) QuantStack                                                 *
 * Copyright (c) Serge Guelton                                              *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef XSIMD_BATCH_HPP
#define XSIMD_BATCH_HPP

#include <cassert>
#include <complex>

#include "../config/xsimd_arch.hpp"
#include "../memory/xsimd_alignment.hpp"
#include "./xsimd_utils.hpp"

namespace xsimd
{

    /**
     * @brief batch of integer or floating point values.
     *
     * Abstract representation of an SIMD register for floating point or integral
     * value.
     *
     * @tparam T the type of the underlying values.
     * @tparam A the architecture this batch is tied too.
     **/
    template <class T, class A = default_arch>
    class batch : public types::simd_register<T, A>
    {
    public:
        static constexpr std::size_t size = sizeof(types::simd_register<T, A>) / sizeof(T); ///< Number of scalar elements in this batch.

        using value_type = T; ///< Type of the scalar elements within this batch.
        using arch_type = A; ///< SIMD Architecture abstracted by this batch.
        using register_type = typename types::simd_register<T, A>::register_type; ///< SIMD register type abstracted by this batch.
        using batch_bool_type = batch_bool<T, A>; ///< Associated batch type used to represented logical operations on this batch.

        // constructors
        batch() = default; ///< Create a batch initialized with undefined values.
        batch(T val) noexcept;
        template <class... Ts>
        batch(T val0, T val1, Ts... vals) noexcept;
        explicit batch(batch_bool_type const& b) noexcept;
        batch(register_type reg) noexcept;

        template <class U>
        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV broadcast(U val) noexcept;

        // memory operators
        template <class U>
        void XSIMD_CALLCONV store_aligned(U* mem) const noexcept;
        template <class U>
        void XSIMD_CALLCONV store_unaligned(U* mem) const noexcept;
        template <class U>
        void XSIMD_CALLCONV store(U* mem, aligned_mode) const noexcept;
        template <class U>
        void XSIMD_CALLCONV store(U* mem, unaligned_mode) const noexcept;

        template <class U>
        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV load_aligned(U const* mem) noexcept;
        template <class U>
        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV load_unaligned(U const* mem) noexcept;
        template <class U>
        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV load(U const* mem, aligned_mode) noexcept;
        template <class U>
        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV load(U const* mem, unaligned_mode) noexcept;

        template <class U, class V>
        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV gather(U const* src, batch<V, arch_type> const& index) noexcept;
        template <class U, class V>
        void XSIMD_CALLCONV scatter(U* dst, batch<V, arch_type> const& index) const noexcept;

        T XSIMD_CALLCONV get(std::size_t i) const noexcept;

        // comparison operators
        inline batch_bool_type XSIMD_CALLCONV operator==(batch XSIMD_CREF other) const noexcept;
        inline batch_bool_type XSIMD_CALLCONV operator!=(batch XSIMD_CREF other) const noexcept;
        inline batch_bool_type XSIMD_CALLCONV operator>=(batch XSIMD_CREF other) const noexcept;
        inline batch_bool_type XSIMD_CALLCONV operator<=(batch XSIMD_CREF other) const noexcept;
        inline batch_bool_type XSIMD_CALLCONV operator>(batch XSIMD_CREF other) const noexcept;
        inline batch_bool_type XSIMD_CALLCONV operator<(batch XSIMD_CREF other) const noexcept;

        // Update operators
        inline batch& XSIMD_CALLCONV operator+=(batch XSIMD_CREF other) noexcept;
        inline batch& XSIMD_CALLCONV operator-=(batch XSIMD_CREF other) noexcept;
        inline batch& XSIMD_CALLCONV operator*=(batch XSIMD_CREF other) noexcept;
        inline batch& XSIMD_CALLCONV operator/=(batch XSIMD_CREF other) noexcept;
        inline batch& XSIMD_CALLCONV operator%=(batch XSIMD_CREF other) noexcept;
        inline batch& XSIMD_CALLCONV operator&=(batch XSIMD_CREF other) noexcept;
        inline batch& XSIMD_CALLCONV operator|=(batch XSIMD_CREF other) noexcept;
        inline batch& XSIMD_CALLCONV operator^=(batch XSIMD_CREF other) noexcept;
        inline batch& XSIMD_CALLCONV operator>>=(int32_t other) noexcept;
        inline batch& XSIMD_CALLCONV operator>>=(batch XSIMD_CREF other) noexcept;
        inline batch& XSIMD_CALLCONV operator<<=(int32_t other) noexcept;
        inline batch& XSIMD_CALLCONV operator<<=(batch XSIMD_CREF other) noexcept;

        // incr/decr operators
        inline batch& XSIMD_CALLCONV operator++() noexcept;
        inline batch& XSIMD_CALLCONV operator--() noexcept;
        inline batch XSIMD_CALLCONV operator++(int) noexcept;
        inline batch XSIMD_CALLCONV operator--(int) noexcept;

        // unary operators
        inline batch_bool_type XSIMD_CALLCONV operator!() const noexcept;
        inline batch XSIMD_CALLCONV operator~() const noexcept;
        inline batch XSIMD_CALLCONV operator-() const noexcept;
        inline batch XSIMD_CALLCONV operator+() const noexcept;

        // arithmetic operators. They are defined as friend to enable automatic
        // conversion of parameters from scalar to batch. Inline implementation
        // is required to avoid warnings.

        /** Shorthand for xsimd::add() */
        friend batch XSIMD_CALLCONV operator+(batch XSIMD_CREF self, batch XSIMD_CREF other) noexcept
        {
            return batch(self) += other;
        }

        /** Shorthand for xsimd::sub() */
        friend batch XSIMD_CALLCONV operator-(batch XSIMD_CREF self, batch XSIMD_CREF other) noexcept
        {
            return batch(self) -= other;
        }

        /** Shorthand for xsimd::mul() */
        friend batch XSIMD_CALLCONV operator*(batch XSIMD_CREF self, batch XSIMD_CREF other) noexcept
        {
            return batch(self) *= other;
        }

        /** Shorthand for xsimd::div() */
        friend batch XSIMD_CALLCONV operator/(batch XSIMD_CREF self, batch XSIMD_CREF other) noexcept
        {
            return batch(self) /= other;
        }

        /** Shorthand for xsimd::mod() */
        friend batch XSIMD_CALLCONV operator%(batch XSIMD_CREF self, batch XSIMD_CREF other) noexcept
        {
            return batch(self) %= other;
        }

        /** Shorthand for xsimd::bitwise_and() */
        friend batch XSIMD_CALLCONV operator&(batch XSIMD_CREF self, batch XSIMD_CREF other) noexcept
        {
            return batch(self) &= other;
        }

        /** Shorthand for xsimd::bitwise_or() */
        friend batch XSIMD_CALLCONV operator|(batch XSIMD_CREF self, batch XSIMD_CREF other) noexcept
        {
            return batch(self) |= other;
        }

        /** Shorthand for xsimd::bitwise_xor() */
        friend batch XSIMD_CALLCONV operator^(batch XSIMD_CREF self, batch XSIMD_CREF other) noexcept
        {
            return batch(self) ^= other;
        }

        /** Shorthand for xsimd::bitwise_rshift() */
        friend batch XSIMD_CALLCONV operator>>(batch XSIMD_CREF self, batch XSIMD_CREF other) noexcept
        {
            return batch(self) >>= other;
        }

        /** Shorthand for xsimd::bitwise_lshift() */
        friend batch XSIMD_CALLCONV operator<<(batch XSIMD_CREF self, batch XSIMD_CREF other) noexcept
        {
            return batch(self) <<= other;
        }

        /** Shorthand for xsimd::bitwise_rshift() */
        friend batch XSIMD_CALLCONV operator>>(batch XSIMD_CREF self, int32_t other) noexcept
        {
            return batch(self) >>= other;
        }

        /** Shorthand for xsimd::bitwise_lshift() */
        friend batch XSIMD_CALLCONV operator<<(batch XSIMD_CREF self, int32_t other) noexcept
        {
            return batch(self) <<= other;
        }

        /** Shorthand for xsimd::logical_and() */
        friend batch XSIMD_CALLCONV operator&&(batch XSIMD_CREF self, batch XSIMD_CREF other) noexcept
        {
            return batch(self).logical_and(other);
        }

        /** Shorthand for xsimd::logical_or() */
        friend batch XSIMD_CALLCONV operator||(batch XSIMD_CREF self, batch XSIMD_CREF other) noexcept
        {
            return batch(self).logical_or(other);
        }

    private:
        batch XSIMD_CALLCONV logical_and(batch XSIMD_CREF other) const noexcept;
        batch XSIMD_CALLCONV logical_or(batch XSIMD_CREF other) const noexcept;
    };

    template <class T, class A>
    constexpr std::size_t batch<T, A>::size;

    /**
     * @brief batch of predicate over scalar or complex values.
     *
     * Abstract representation of a predicate over SIMD register for scalar or
     * complex values.
     *
     * @tparam T the type of the predicated values.
     * @tparam A the architecture this batch is tied too.
     **/
    template <class T, class A = default_arch>
    class batch_bool : public types::get_bool_simd_register_t<T, A>
    {
        using base_type = types::get_bool_simd_register_t<T, A>;

    public:
        static constexpr std::size_t size = sizeof(types::simd_register<T, A>) / sizeof(T); ///< Number of scalar elements in this batch.

        using value_type = bool; ///< Type of the scalar elements within this batch.
        using arch_type = A; ///< SIMD Architecture abstracted by this batch.
        using register_type = typename base_type::register_type; ///< SIMD register type abstracted by this batch.
        using batch_type = batch<T, A>; ///< Associated batch type this batch represents logical operations for.

        // constructors
        batch_bool() = default; ///< Create a batch initialized with undefined values.
        batch_bool(bool val) noexcept;
        batch_bool(register_type reg) noexcept;
        template <class... Ts>
        batch_bool(bool val0, bool val1, Ts... vals) noexcept;

        template <class Tp>
        batch_bool(Tp const*) = delete;

        // memory operators
        void XSIMD_CALLCONV store_aligned(bool* mem) const noexcept;
        void XSIMD_CALLCONV store_unaligned(bool* mem) const noexcept;
        XSIMD_NO_DISCARD static batch_bool XSIMD_CALLCONV load_aligned(bool const* mem) noexcept;
        XSIMD_NO_DISCARD static batch_bool XSIMD_CALLCONV load_unaligned(bool const* mem) noexcept;

        bool XSIMD_CALLCONV get(std::size_t i) const noexcept;

        // mask operations
        uint64_t XSIMD_CALLCONV mask() const noexcept;
        static batch_bool XSIMD_CALLCONV from_mask(uint64_t mask) noexcept;

        // comparison operators
        batch_bool XSIMD_CALLCONV operator==(batch_bool XSIMD_CREF other) const noexcept;
        batch_bool XSIMD_CALLCONV operator!=(batch_bool XSIMD_CREF other) const noexcept;

        // logical operators
        batch_bool XSIMD_CALLCONV operator~() const noexcept;
        batch_bool XSIMD_CALLCONV operator!() const noexcept;
        batch_bool XSIMD_CALLCONV operator&(batch_bool XSIMD_CREF other) const noexcept;
        batch_bool XSIMD_CALLCONV operator|(batch_bool XSIMD_CREF other) const noexcept;
        batch_bool XSIMD_CALLCONV operator^(batch_bool XSIMD_CREF other) const noexcept;
        batch_bool XSIMD_CALLCONV operator&&(batch_bool XSIMD_CREF other) const noexcept;
        batch_bool XSIMD_CALLCONV operator||(batch_bool XSIMD_CREF other) const noexcept;

        // update operators
        batch_bool& XSIMD_CALLCONV operator&=(batch_bool XSIMD_CREF other) const noexcept { return (*this) = (*this) & other; }
        batch_bool& XSIMD_CALLCONV operator|=(batch_bool XSIMD_CREF other) const noexcept { return (*this) = (*this) | other; }
        batch_bool& XSIMD_CALLCONV operator^=(batch_bool XSIMD_CREF other) const noexcept { return (*this) = (*this) ^ other; }

    private:
        template <class U, class... V, size_t I, size_t... Is>
        static register_type XSIMD_CALLCONV make_register(detail::index_sequence<I, Is...>, U u, V... v) noexcept;

        template <class... V>
        static register_type XSIMD_CALLCONV make_register(detail::index_sequence<>, V... v) noexcept;
    };

    template <class T, class A>
    constexpr std::size_t batch_bool<T, A>::size;

    /**
     * @brief batch of complex values.
     *
     * Abstract representation of an SIMD register for complex values.
     *
     * @tparam T the type of the underlying values.
     * @tparam A the architecture this batch is tied too.
     **/
    template <class T, class A>
    class batch<std::complex<T>, A>
    {
    public:
        using value_type = std::complex<T>; ///< Type of the complex elements within this batch.
        using real_batch = batch<T, A>; ///< Type of the scalar elements within this batch.
        using arch_type = A; ///< SIMD Architecture abstracted by this batch.
        using batch_bool_type = batch_bool<T, A>; ///< Associated batch type used to represented logical operations on this batch.

        static constexpr std::size_t size = real_batch::size; ///< Number of complex elements in this batch.

        // constructors
        batch() = default; ///< Create a batch initialized with undefined values.
        batch(value_type const& val) noexcept;
        batch(real_batch const& real, real_batch const& imag) noexcept;

        batch(real_batch const& real) noexcept;
        batch(T val) noexcept;
        template <class... Ts>
        batch(value_type val0, value_type val1, Ts... vals) noexcept;
        explicit batch(batch_bool_type const& b) noexcept;

        // memory operators
        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV load_aligned(const T* real_src, const T* imag_src = nullptr) noexcept;
        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV load_unaligned(const T* real_src, const T* imag_src = nullptr) noexcept;
        void XSIMD_CALLCONV store_aligned(T* real_dst, T* imag_dst) const noexcept;
        void XSIMD_CALLCONV store_unaligned(T* real_dst, T* imag_dst) const noexcept;

        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV load_aligned(const value_type* src) noexcept;
        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV load_unaligned(const value_type* src) noexcept;
        void XSIMD_CALLCONV store_aligned(value_type* dst) const noexcept;
        void XSIMD_CALLCONV store_unaligned(value_type* dst) const noexcept;

        template <class U>
        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV load(U const* mem, aligned_mode) noexcept;
        template <class U>
        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV load(U const* mem, unaligned_mode) noexcept;
        template <class U>
        void XSIMD_CALLCONV store(U* mem, aligned_mode) const noexcept;
        template <class U>
        void XSIMD_CALLCONV store(U* mem, unaligned_mode) const noexcept;

        real_batch XSIMD_CALLCONV real() const noexcept;
        real_batch XSIMD_CALLCONV imag() const noexcept;

        value_type XSIMD_CALLCONV get(std::size_t i) const noexcept;

#ifdef XSIMD_ENABLE_XTL_COMPLEX
        // xtl-related methods
        template <bool i3ec>
        batch(xtl::xcomplex<T, T, i3ec> const& val) noexcept;
        template <bool i3ec, class... Ts>
        batch(xtl::xcomplex<T, T, i3ec> val0, xtl::xcomplex<T, T, i3ec> val1, Ts... vals) noexcept;

        template <bool i3ec>
        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV load_aligned(const xtl::xcomplex<T, T, i3ec>* src) noexcept;
        template <bool i3ec>
        XSIMD_NO_DISCARD static batch XSIMD_CALLCONV load_unaligned(const xtl::xcomplex<T, T, i3ec>* src) noexcept;
        template <bool i3ec>
        void XSIMD_CALLCONV store_aligned(xtl::xcomplex<T, T, i3ec>* dst) const noexcept;
        template <bool i3ec>
        void XSIMD_CALLCONV store_unaligned(xtl::xcomplex<T, T, i3ec>* dst) const noexcept;
#endif

        // comparison operators
        batch_bool<T, A> XSIMD_CALLCONV operator==(batch const& other) const noexcept;
        batch_bool<T, A> XSIMD_CALLCONV operator!=(batch const& other) const noexcept;

        // Update operators
        batch& XSIMD_CALLCONV operator+=(batch const& other) noexcept;
        batch& XSIMD_CALLCONV operator-=(batch const& other) noexcept;
        batch& XSIMD_CALLCONV operator*=(batch const& other) noexcept;
        batch& XSIMD_CALLCONV operator/=(batch const& other) noexcept;

        // incr/decr operators
        batch& XSIMD_CALLCONV operator++() noexcept;
        batch& XSIMD_CALLCONV operator--() noexcept;
        batch XSIMD_CALLCONV operator++(int) noexcept;
        batch XSIMD_CALLCONV operator--(int) noexcept;

        // unary operators
        batch_bool_type XSIMD_CALLCONV operator!() const noexcept;
        batch XSIMD_CALLCONV operator~() const noexcept;
        batch XSIMD_CALLCONV operator-() const noexcept;
        batch XSIMD_CALLCONV operator+() const noexcept;

        // arithmetic operators. They are defined as friend to enable automatic
        // conversion of parameters from scalar to batch

        /** Shorthand for xsimd::add() */
        friend batch XSIMD_CALLCONV operator+(batch const& self, batch const& other) noexcept
        {
            return batch(self) += other;
        }

        /** Shorthand for xsimd::sub() */
        friend batch XSIMD_CALLCONV operator-(batch const& self, batch const& other) noexcept
        {
            return batch(self) -= other;
        }

        /** Shorthand for xsimd::mul() */
        friend batch XSIMD_CALLCONV operator*(batch const& self, batch const& other) noexcept
        {
            return batch(self) *= other;
        }

        /** Shorthand for xsimd::div() */
        friend batch XSIMD_CALLCONV operator/(batch const& self, batch const& other) noexcept
        {
            return batch(self) /= other;
        }

    private:
        real_batch m_real;
        real_batch m_imag;
    };

    template <class T, class A>
    constexpr std::size_t batch<std::complex<T>, A>::size;

#ifdef XSIMD_ENABLE_XTL_COMPLEX
    template <typename T, bool i3ec, typename A>
    struct batch<xtl::xcomplex<T, T, i3ec>, A>
    {
        static_assert(std::is_same<T, void>::value,
                      "Please use batch<std::complex<T>, A> initialized from xtl::complex instead");
    };
#endif
}

#include "../arch/xsimd_isa.hpp"
#include "../types/xsimd_batch_constant.hpp"

namespace xsimd
{

    /**
     * Create a batch with all element initialized to \c val.
     */
    template <class T, class A>
    inline batch<T, A>::batch(T val) noexcept
        : types::simd_register<T, A>(kernel::broadcast<A>(val, A {}))
    {
    }

    /**
     * Create a batch with elements initialized from \c val0, \c val1, \c vals...
     * There must be exactly \c size elements in total.
     */
    template <class T, class A>
    template <class... Ts>
    inline batch<T, A>::batch(T val0, T val1, Ts... vals) noexcept
        : batch(kernel::set<A>(batch {}, A {}, val0, val1, static_cast<T>(vals)...))
    {
        static_assert(sizeof...(Ts) + 2 == size, "The constructor requires as many arguments as batch elements.");
    }

    /**
     * Converts a \c bool_batch to a \c batch where each element is
     * set to 0xFF..FF (resp. 0x00..00) if the corresponding element is `true`
     * (resp. `false`).
     */
    template <class T, class A>
    inline batch<T, A>::batch(batch_bool<T, A> const& b) noexcept
        : batch(kernel::from_bool(b, A {}))
    {
    }

    /**
     * Wraps a compatible native simd register as a \c batch. This is generally not needed but
     * becomes handy when doing architecture-specific operations.
     */
    template <class T, class A>
    inline batch<T, A>::batch(register_type reg) noexcept
        : types::simd_register<T, A>({ reg })
    {
    }

    /**
     * Equivalent to batch::batch(T val).
     */
    template <class T, class A>
    template <class U>
    XSIMD_NO_DISCARD XSIMD_FORCEINLINE batch<T, A> XSIMD_CALLCONV batch<T, A>::broadcast(U val) noexcept
    {
        return batch(static_cast<T>(val));
    }

    /**************************
     * batch memory operators *
     **************************/

    /**
     * Copy content of this batch to the buffer \c mem. The
     * memory needs to be aligned.
     */
    template <class T, class A>
    template <class U>
    XSIMD_FORCEINLINE void XSIMD_CALLCONV batch<T, A>::store_aligned(U* mem) const noexcept
    {
        kernel::store_aligned<A>(mem, *this, A {});
    }

    /**
     * Copy content of this batch to the buffer \c mem. The
     * memory does not need to be aligned.
     */
    template <class T, class A>
    template <class U>
    XSIMD_FORCEINLINE void XSIMD_CALLCONV batch<T, A>::store_unaligned(U* mem) const noexcept
    {
        kernel::store_unaligned<A>(mem, *this, A {});
    }

    /**
     * Equivalent to batch::store_aligned()
     */
    template <class T, class A>
    template <class U>
    XSIMD_FORCEINLINE void XSIMD_CALLCONV batch<T, A>::store(U* mem, aligned_mode) const noexcept
    {
        return store_aligned(mem);
    }

    /**
     * Equivalent to batch::store_unaligned()
     */
    template <class T, class A>
    template <class U>
    XSIMD_FORCEINLINE void XSIMD_CALLCONV batch<T, A>::store(U* mem, unaligned_mode) const noexcept
    {
        return store_unaligned(mem);
    }

    /**
     * Loading from aligned memory. May involve a conversion if \c U is different
     * from \c T.
     */
    template <class T, class A>
    template <class U>
    XSIMD_FORCEINLINE batch<T, A> XSIMD_CALLCONV batch<T, A>::load_aligned(U const* mem) noexcept
    {
        return kernel::load_aligned<A>(mem, kernel::convert<T> {}, A {});
    }

    /**
     * Loading from unaligned memory. May involve a conversion if \c U is different
     * from \c T.
     */
    template <class T, class A>
    template <class U>
    XSIMD_FORCEINLINE batch<T, A> XSIMD_CALLCONV batch<T, A>::load_unaligned(U const* mem) noexcept
    {
        return kernel::load_unaligned<A>(mem, kernel::convert<T> {}, A {});
    }

    /**
     * Equivalent to batch::load_aligned()
     */
    template <class T, class A>
    template <class U>
    XSIMD_FORCEINLINE batch<T, A> XSIMD_CALLCONV batch<T, A>::load(U const* mem, aligned_mode) noexcept
    {
        return load_aligned(mem);
    }

    /**
     * Equivalent to batch::load_unaligned()
     */
    template <class T, class A>
    template <class U>
    XSIMD_FORCEINLINE batch<T, A> XSIMD_CALLCONV batch<T, A>::load(U const* mem, unaligned_mode) noexcept
    {
        return load_unaligned(mem);
    }

    /**
     * Create a new batch gathering elements starting at address \c src and
     * offset by each element in \c index.
     * If \c T is not of the same size as \c U, a \c static_cast is performed
     * at element gather time.
     */
    template <class T, class A>
    template <typename U, typename V>
    XSIMD_FORCEINLINE batch<T, A> XSIMD_CALLCONV batch<T, A>::gather(U const* src, batch<V, A> const& index) noexcept
    {
        static_assert(std::is_convertible<T, U>::value, "Can't convert from src to this batch's type!");
        return kernel::gather(batch {}, src, index, A {});
    }

    /**
     * Scatter elements from this batch into addresses starting at \c dst
     * and offset by each element in \c index.
     * If \c T is not of the same size as \c U, a \c static_cast is performed
     * at element scatter time.
     */
    template <class T, class A>
    template <class U, class V>
    XSIMD_FORCEINLINE void XSIMD_CALLCONV batch<T, A>::scatter(U* dst, batch<V, A> const& index) const noexcept
    {
        static_assert(std::is_convertible<T, U>::value, "Can't convert from this batch's type to dst!");
        kernel::scatter<A>(*this, dst, index, A {});
    }

    /**
     * Retrieve the \c i th scalar element in this batch.
     *
     * \c warning This is very inefficient and should only be used for debugging purpose.
     */
    template <class T, class A>
    XSIMD_FORCEINLINE T XSIMD_CALLCONV batch<T, A>::get(std::size_t i) const noexcept
    {
        return kernel::get(*this, i, A {});
    }

    /******************************
     * batch comparison operators *
     ******************************/

    /**
     * Shorthand for xsimd::eq()
     */
    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch<T, A>::operator==(batch<T, A> XSIMD_CREF other) const noexcept
    {
        return kernel::eq<A>(*this, other, A {});
    }

    /**
     * Shorthand for xsimd::neq()
     */
    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch<T, A>::operator!=(batch<T, A> XSIMD_CREF other) const noexcept
    {
        return kernel::neq<A>(*this, other, A {});
    }

    /**
     * Shorthand for xsimd::ge()
     */
    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch<T, A>::operator>=(batch<T, A> XSIMD_CREF other) const noexcept
    {
        return kernel::ge<A>(*this, other, A {});
    }

    /**
     * Shorthand for xsimd::le()
     */
    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch<T, A>::operator<=(batch<T, A> XSIMD_CREF other) const noexcept
    {
        return kernel::le<A>(*this, other, A {});
    }

    /**
     * Shorthand for xsimd::gt()
     */
    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch<T, A>::operator>(batch<T, A> XSIMD_CREF other) const noexcept
    {
        return kernel::gt<A>(*this, other, A {});
    }

    /**
     * Shorthand for xsimd::lt()
     */
    template <class T, class A>
    inline batch_bool<T, A> XSIMD_CALLCONV batch<T, A>::operator<(batch<T, A> XSIMD_CREF other) const noexcept
    {
        return kernel::lt<A>(*this, other, A {});
    }

    /**************************
     * batch update operators *
     **************************/

    template <class T, class A>
    inline batch<T, A>& XSIMD_CALLCONV batch<T, A>::operator+=(batch<T, A> XSIMD_CREF other) noexcept
    {
        return *this = kernel::add<A>(*this, other, A {});
    }

    template <class T, class A>
    inline batch<T, A>& XSIMD_CALLCONV batch<T, A>::operator-=(batch<T, A> XSIMD_CREF other) noexcept
    {
        return *this = kernel::sub<A>(*this, other, A {});
    }

    template <class T, class A>
    inline batch<T, A>& XSIMD_CALLCONV batch<T, A>::operator*=(batch<T, A> XSIMD_CREF other) noexcept
    {
        return *this = kernel::mul<A>(*this, other, A {});
    }

    template <class T, class A>
    inline batch<T, A>& XSIMD_CALLCONV batch<T, A>::operator/=(batch<T, A> XSIMD_CREF other) noexcept
    {
        return *this = kernel::div<A>(*this, other, A {});
    }

    template <class T, class A>
    inline batch<T, A>& XSIMD_CALLCONV batch<T, A>::operator%=(batch<T, A> XSIMD_CREF other) noexcept
    {
        return *this = kernel::mod<A>(*this, other, A {});
    }

    template <class T, class A>
    inline batch<T, A>& XSIMD_CALLCONV batch<T, A>::operator&=(batch<T, A> XSIMD_CREF other) noexcept
    {
        return *this = kernel::bitwise_and<A>(*this, other, A {});
    }

    template <class T, class A>
    inline batch<T, A>& XSIMD_CALLCONV batch<T, A>::operator|=(batch<T, A> XSIMD_CREF other) noexcept
    {
        return *this = kernel::bitwise_or<A>(*this, other, A {});
    }

    template <class T, class A>
    inline batch<T, A>& XSIMD_CALLCONV batch<T, A>::operator^=(batch<T, A> XSIMD_CREF other) noexcept
    {
        return *this = kernel::bitwise_xor<A>(*this, other, A {});
    }

    template <class T, class A>
    inline batch<T, A>& XSIMD_CALLCONV batch<T, A>::operator>>=(batch<T, A> XSIMD_CREF other) noexcept
    {
        return *this = kernel::bitwise_rshift<A>(*this, other, A {});
    }

    template <class T, class A>
    inline batch<T, A>& XSIMD_CALLCONV batch<T, A>::operator<<=(batch<T, A> XSIMD_CREF other) noexcept
    {
        return *this = kernel::bitwise_lshift<A>(*this, other, A {});
    }

    template <class T, class A>
    inline batch<T, A>& XSIMD_CALLCONV batch<T, A>::operator>>=(int32_t other) noexcept
    {
        return *this = kernel::bitwise_rshift<A>(*this, other, A {});
    }

    template <class T, class A>
    inline batch<T, A>& XSIMD_CALLCONV batch<T, A>::operator<<=(int32_t other) noexcept
    {
        return *this = kernel::bitwise_lshift<A>(*this, other, A {});
    }

    /*****************************
     * batch incr/decr operators *
     *****************************/

    template <class T, class A>
    inline batch<T, A>& batch<T, A>::operator++() noexcept
    {
        return operator+=(1);
    }

    template <class T, class A>
    inline batch<T, A>& batch<T, A>::operator--() noexcept
    {
        return operator-=(1);
    }

    template <class T, class A>
    inline batch<T, A> batch<T, A>::operator++(int) noexcept
    {
        batch<T, A> copy(*this);
        operator+=(1);
        return copy;
    }

    template <class T, class A>
    inline batch<T, A> batch<T, A>::operator--(int) noexcept
    {
        batch copy(*this);
        operator-=(1);
        return copy;
    }

    /*************************
     * batch unary operators *
     *************************/

    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch<T, A>::operator!() const noexcept
    {
        return kernel::eq<A>(*this, batch(0), A {});
    }

    template <class T, class A>
    XSIMD_FORCEINLINE batch<T, A> XSIMD_CALLCONV batch<T, A>::operator~() const noexcept
    {
        return kernel::bitwise_not<A>(*this, A {});
    }

    template <class T, class A>
    XSIMD_FORCEINLINE batch<T, A> XSIMD_CALLCONV batch<T, A>::operator-() const noexcept
    {
        return kernel::neg<A>(*this, A {});
    }

    template <class T, class A>
    XSIMD_FORCEINLINE batch<T, A> XSIMD_CALLCONV batch<T, A>::operator+() const noexcept
    {
        return *this;
    }

    /************************
     * batch private method *
     ************************/

    template <class T, class A>
    XSIMD_FORCEINLINE batch<T, A> XSIMD_CALLCONV batch<T, A>::logical_and(batch<T, A> XSIMD_CREF other) const noexcept
    {
        return kernel::logical_and<A>(*this, other, A());
    }

    template <class T, class A>
    XSIMD_FORCEINLINE batch<T, A> XSIMD_CALLCONV batch<T, A>::logical_or(batch<T, A> XSIMD_CREF other) const noexcept
    {
        return kernel::logical_or<A>(*this, other, A());
    }

    /***************************
     * batch_bool constructors *
     ***************************/

    template <class T, class A>
    inline batch_bool<T, A>::batch_bool(register_type reg) noexcept
        : types::get_bool_simd_register_t<T, A>({ reg })
    {
    }

    template <class T, class A>
    template <class... Ts>
    inline batch_bool<T, A>::batch_bool(bool val0, bool val1, Ts... vals) noexcept
        : batch_bool(kernel::set<A>(batch_bool {}, A {}, val0, val1, static_cast<bool>(vals)...))
    {
        static_assert(sizeof...(Ts) + 2 == size, "The constructor requires as many arguments as batch elements.");
    }

    /*******************************
     * batch_bool memory operators *
     *******************************/

    template <class T, class A>
    XSIMD_FORCEINLINE void XSIMD_CALLCONV batch_bool<T, A>::store_aligned(bool* mem) const noexcept
    {
        kernel::store(*this, mem, A {});
    }

    template <class T, class A>
    XSIMD_FORCEINLINE void XSIMD_CALLCONV batch_bool<T, A>::store_unaligned(bool* mem) const noexcept
    {
        store_aligned(mem);
    }

    template <class T, class A>
    inline batch_bool<T, A> XSIMD_CALLCONV batch_bool<T, A>::load_aligned(bool const* mem) noexcept
    {
        batch_type ref(0);
        alignas(A::alignment()) T buffer[size];
        for (std::size_t i = 0; i < size; ++i)
            buffer[i] = mem[i] ? 1 : 0;
        return ref != batch_type::load_aligned(&buffer[0]);
    }

    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch_bool<T, A>::load_unaligned(bool const* mem) noexcept
    {
        return load_aligned(mem);
    }

    /**
     * Extract a scalar mask representation from this @c batch_bool.
     *
     * @return bit mask
     */
    template <class T, class A>
    XSIMD_FORCEINLINE uint64_t XSIMD_CALLCONV batch_bool<T, A>::mask() const noexcept
    {
        return kernel::mask(*this, A {});
    }

    /**
     * Extract a scalar mask representation from this @c batch_bool.
     *
     * @return bit mask
     */
    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch_bool<T, A>::from_mask(uint64_t mask) noexcept
    {
        return kernel::from_mask(batch_bool<T, A>(), mask, A {});
    }

    template <class T, class A>
    XSIMD_FORCEINLINE bool XSIMD_CALLCONV batch_bool<T, A>::get(std::size_t i) const noexcept
    {
        return kernel::get(*this, i, A {});
    }

    /***********************************
     * batch_bool comparison operators *
     ***********************************/

    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch_bool<T, A>::operator==(batch_bool<T, A> XSIMD_CREF other) const noexcept
    {
        return kernel::eq<A>(*this, other, A {}).data;
    }

    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch_bool<T, A>::operator!=(batch_bool<T, A> XSIMD_CREF other) const noexcept
    {
        return kernel::neq<A>(*this, other, A {}).data;
    }

    /********************************
     * batch_bool logical operators *
     ********************************/

    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch_bool<T, A>::operator~() const noexcept
    {
        return kernel::bitwise_not<A>(*this, A {}).data;
    }

    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch_bool<T, A>::operator!() const noexcept
    {
        return operator==(batch_bool(false));
    }

    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch_bool<T, A>::operator&(batch_bool<T, A> XSIMD_CREF other) const noexcept
    {
        return kernel::bitwise_and<A>(*this, other, A {}).data;
    }

    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch_bool<T, A>::operator|(batch_bool<T, A> XSIMD_CREF other) const noexcept
    {
        return kernel::bitwise_or<A>(*this, other, A {}).data;
    }

    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch_bool<T, A>::operator^(batch_bool<T, A> XSIMD_CREF other) const noexcept
    {
        return kernel::bitwise_xor<A>(*this, other, A {}).data;
    }

    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch_bool<T, A>::operator&&(batch_bool XSIMD_CREF other) const noexcept
    {
        return operator&(other);
    }

    template <class T, class A>
    XSIMD_FORCEINLINE batch_bool<T, A> XSIMD_CALLCONV batch_bool<T, A>::operator||(batch_bool XSIMD_CREF other) const noexcept
    {
        return operator|(other);
    }

    /******************************
     * batch_bool private methods *
     ******************************/

    template <class T, class A>
    inline batch_bool<T, A>::batch_bool(bool val) noexcept
        : base_type { make_register(detail::make_index_sequence<size - 1>(), val) }
    {
    }

    template <class T, class A>
    template <class U, class... V, size_t I, size_t... Is>
    XSIMD_FORCEINLINE auto XSIMD_CALLCONV batch_bool<T, A>::make_register(detail::index_sequence<I, Is...>, U u, V... v) noexcept -> register_type
    {
        return make_register(detail::index_sequence<Is...>(), u, u, v...);
    }

    template <class T, class A>
    template <class... V>
    XSIMD_FORCEINLINE auto XSIMD_CALLCONV batch_bool<T, A>::make_register(detail::index_sequence<>, V... v) noexcept -> register_type
    {
        return kernel::set<A>(batch_bool<T, A>(), A {}, v...).data;
    }

    /*******************************
     * batch<complex> constructors *
     *******************************/

    template <class T, class A>
    inline batch<std::complex<T>, A>::batch(value_type const& val) noexcept
        : m_real(val.real())
        , m_imag(val.imag())
    {
    }

    template <class T, class A>
    inline batch<std::complex<T>, A>::batch(real_batch const& real, real_batch const& imag) noexcept
        : m_real(real)
        , m_imag(imag)
    {
    }

    template <class T, class A>
    inline batch<std::complex<T>, A>::batch(real_batch const& real) noexcept
        : m_real(real)
        , m_imag(0)
    {
    }

    template <class T, class A>
    inline batch<std::complex<T>, A>::batch(T val) noexcept
        : m_real(val)
        , m_imag(0)
    {
    }

    template <class T, class A>
    template <class... Ts>
    inline batch<std::complex<T>, A>::batch(value_type val0, value_type val1, Ts... vals) noexcept
        : batch(kernel::set<A>(batch {}, A {}, val0, val1, static_cast<value_type>(vals)...))
    {
        static_assert(sizeof...(Ts) + 2 == size, "as many arguments as batch elements");
    }

    template <class T, class A>
    inline batch<std::complex<T>, A>::batch(batch_bool_type const& b) noexcept
        : m_real(b)
        , m_imag(0)
    {
    }

    /***********************************
     * batch<complex> memory operators *
     ***********************************/

    template <class T, class A>
    inline batch<std::complex<T>, A> XSIMD_CALLCONV batch<std::complex<T>, A>::load_aligned(const T* real_src, const T* imag_src) noexcept
    {
        return { batch<T, A>::load_aligned(real_src), imag_src ? batch<T, A>::load_aligned(imag_src) : batch<T, A>(0) };
    }
    template <class T, class A>
    inline batch<std::complex<T>, A> XSIMD_CALLCONV batch<std::complex<T>, A>::load_unaligned(const T* real_src, const T* imag_src) noexcept
    {
        return { batch<T, A>::load_unaligned(real_src), imag_src ? batch<T, A>::load_unaligned(imag_src) : batch<T, A>(0) };
    }

    template <class T, class A>
    inline batch<std::complex<T>, A> XSIMD_CALLCONV batch<std::complex<T>, A>::load_aligned(const value_type* src) noexcept
    {
        return kernel::load_complex_aligned<A>(src, kernel::convert<value_type> {}, A {});
    }

    template <class T, class A>
    inline batch<std::complex<T>, A> XSIMD_CALLCONV batch<std::complex<T>, A>::load_unaligned(const value_type* src) noexcept
    {
        return kernel::load_complex_unaligned<A>(src, kernel::convert<value_type> {}, A {});
    }

    template <class T, class A>
    inline void XSIMD_CALLCONV batch<std::complex<T>, A>::store_aligned(value_type* dst) const noexcept
    {
        return kernel::store_complex_aligned(dst, *this, A {});
    }

    template <class T, class A>
    inline void XSIMD_CALLCONV batch<std::complex<T>, A>::store_unaligned(value_type* dst) const noexcept
    {
        return kernel::store_complex_unaligned(dst, *this, A {});
    }

    template <class T, class A>
    inline void XSIMD_CALLCONV batch<std::complex<T>, A>::store_aligned(T* real_dst, T* imag_dst) const noexcept
    {
        m_real.store_aligned(real_dst);
        m_imag.store_aligned(imag_dst);
    }

    template <class T, class A>
    inline void XSIMD_CALLCONV batch<std::complex<T>, A>::store_unaligned(T* real_dst, T* imag_dst) const noexcept
    {
        m_real.store_unaligned(real_dst);
        m_imag.store_unaligned(imag_dst);
    }

    template <class T, class A>
    template <class U>
    inline batch<std::complex<T>, A> XSIMD_CALLCONV batch<std::complex<T>, A>::load(U const* mem, aligned_mode) noexcept
    {
        return load_aligned(mem);
    }

    template <class T, class A>
    template <class U>
    inline batch<std::complex<T>, A> XSIMD_CALLCONV batch<std::complex<T>, A>::load(U const* mem, unaligned_mode) noexcept
    {
        return load_unaligned(mem);
    }

    template <class T, class A>
    template <class U>
    inline void XSIMD_CALLCONV batch<std::complex<T>, A>::store(U* mem, aligned_mode) const noexcept
    {
        return store_aligned(mem);
    }

    template <class T, class A>
    template <class U>
    inline void XSIMD_CALLCONV batch<std::complex<T>, A>::store(U* mem, unaligned_mode) const noexcept
    {
        return store_unaligned(mem);
    }

    template <class T, class A>
    inline auto XSIMD_CALLCONV batch<std::complex<T>, A>::real() const noexcept -> real_batch
    {
        return m_real;
    }

    template <class T, class A>
    inline auto XSIMD_CALLCONV batch<std::complex<T>, A>::imag() const noexcept -> real_batch
    {
        return m_imag;
    }

    template <class T, class A>
    inline auto XSIMD_CALLCONV batch<std::complex<T>, A>::get(std::size_t i) const noexcept -> value_type
    {
        return kernel::get(*this, i, A {});
    }

    /**************************************
     * batch<complex> xtl-related methods *
     **************************************/

#ifdef XSIMD_ENABLE_XTL_COMPLEX

    template <class T, class A>
    template <bool i3ec>
    inline batch<std::complex<T>, A>::batch(xtl::xcomplex<T, T, i3ec> const& val) noexcept
        : m_real(val.real())
        , m_imag(val.imag())
    {
    }

    template <class T, class A>
    template <bool i3ec, class... Ts>
    inline batch<std::complex<T>, A>::batch(xtl::xcomplex<T, T, i3ec> val0, xtl::xcomplex<T, T, i3ec> val1, Ts... vals) noexcept
        : batch(kernel::set<A>(batch {}, A {}, val0, val1, static_cast<xtl::xcomplex<T, T, i3ec>>(vals)...))
    {
        static_assert(sizeof...(Ts) + 2 == size, "as many arguments as batch elements");
    }

    // Memory layout of an xcomplex and std::complex are the same when xcomplex
    // stores values and not reference. Unfortunately, this breaks strict
    // aliasing...

    template <class T, class A>
    template <bool i3ec>
    inline batch<std::complex<T>, A> XSIMD_CALLCONV batch<std::complex<T>, A>::load_aligned(const xtl::xcomplex<T, T, i3ec>* src) noexcept
    {
        return load_aligned(reinterpret_cast<std::complex<T> const*>(src));
    }

    template <class T, class A>
    template <bool i3ec>
    inline batch<std::complex<T>, A> XSIMD_CALLCONV batch<std::complex<T>, A>::load_unaligned(const xtl::xcomplex<T, T, i3ec>* src) noexcept
    {
        return load_unaligned(reinterpret_cast<std::complex<T> const*>(src));
    }

    template <class T, class A>
    template <bool i3ec>
    inline void XSIMD_CALLCONV batch<std::complex<T>, A>::store_aligned(xtl::xcomplex<T, T, i3ec>* dst) const noexcept
    {
        store_aligned(reinterpret_cast<std::complex<T>*>(dst));
    }

    template <class T, class A>
    template <bool i3ec>
    inline void XSIMD_CALLCONV batch<std::complex<T>, A>::store_unaligned(xtl::xcomplex<T, T, i3ec>* dst) const noexcept
    {
        store_unaligned(reinterpret_cast<std::complex<T>*>(dst));
    }

#endif

    /***************************************
     * batch<complex> comparison operators *
     ***************************************/

    template <class T, class A>
    inline batch_bool<T, A> XSIMD_CALLCONV batch<std::complex<T>, A>::operator==(batch const& other) const noexcept
    {
        return m_real == other.m_real && m_imag == other.m_imag;
    }

    template <class T, class A>
    inline batch_bool<T, A> XSIMD_CALLCONV batch<std::complex<T>, A>::operator!=(batch const& other) const noexcept
    {
        return m_real != other.m_real || m_imag != other.m_imag;
    }

    /***********************************
     * batch<complex> update operators *
     ***********************************/

    template <class T, class A>
    inline batch<std::complex<T>, A>& XSIMD_CALLCONV batch<std::complex<T>, A>::operator+=(batch const& other) noexcept
    {
        m_real += other.m_real;
        m_imag += other.m_imag;
        return *this;
    }

    template <class T, class A>
    inline batch<std::complex<T>, A>& XSIMD_CALLCONV batch<std::complex<T>, A>::operator-=(batch const& other) noexcept
    {
        m_real -= other.m_real;
        m_imag -= other.m_imag;
        return *this;
    }

    template <class T, class A>
    inline batch<std::complex<T>, A>& XSIMD_CALLCONV batch<std::complex<T>, A>::operator*=(batch const& other) noexcept
    {
        real_batch new_real = real() * other.real() - imag() * other.imag();
        real_batch new_imag = real() * other.imag() + imag() * other.real();
        m_real = new_real;
        m_imag = new_imag;
        return *this;
    }

    template <class T, class A>
    inline batch<std::complex<T>, A>& XSIMD_CALLCONV batch<std::complex<T>, A>::operator/=(batch const& other) noexcept
    {
        real_batch a = real();
        real_batch b = imag();
        real_batch c = other.real();
        real_batch d = other.imag();
        real_batch e = c * c + d * d;
        m_real = (c * a + d * b) / e;
        m_imag = (c * b - d * a) / e;
        return *this;
    }

    /**************************************
     * batch<complex> incr/decr operators *
     **************************************/

    template <class T, class A>
    inline batch<std::complex<T>, A>& XSIMD_CALLCONV batch<std::complex<T>, A>::operator++() noexcept
    {
        return operator+=(1);
    }

    template <class T, class A>
    inline batch<std::complex<T>, A>& XSIMD_CALLCONV batch<std::complex<T>, A>::operator--() noexcept
    {
        return operator-=(1);
    }

    template <class T, class A>
    inline batch<std::complex<T>, A> XSIMD_CALLCONV batch<std::complex<T>, A>::operator++(int) noexcept
    {
        batch copy(*this);
        operator+=(1);
        return copy;
    }

    template <class T, class A>
    inline batch<std::complex<T>, A> batch<std::complex<T>, A>::operator--(int) noexcept
    {
        batch copy(*this);
        operator-=(1);
        return copy;
    }

    /**********************************
     * batch<complex> unary operators *
     **********************************/

    template <class T, class A>
    inline batch_bool<T, A> XSIMD_CALLCONV batch<std::complex<T>, A>::operator!() const noexcept
    {
        return operator==(batch(0));
    }

    template <class T, class A>
    inline batch<std::complex<T>, A> XSIMD_CALLCONV batch<std::complex<T>, A>::operator~() const noexcept
    {
        return { ~m_real, ~m_imag };
    }

    template <class T, class A>
    inline batch<std::complex<T>, A> XSIMD_CALLCONV batch<std::complex<T>, A>::operator-() const noexcept
    {
        return { -m_real, -m_imag };
    }

    template <class T, class A>
    inline batch<std::complex<T>, A> XSIMD_CALLCONV batch<std::complex<T>, A>::operator+() const noexcept
    {
        return { +m_real, +m_imag };
    }

    /**********************************
     * size type aliases
     **********************************/

    namespace details
    {
        template <typename T, std::size_t N, class ArchList>
        struct sized_batch;

        template <typename T, std::size_t N>
        struct sized_batch<T, N, xsimd::arch_list<>>
        {
            using type = void;
        };

        template <typename T, std::size_t N, class Arch, class... Archs>
        struct sized_batch<T, N, xsimd::arch_list<Arch, Archs...>>
        {
            using type = typename std::conditional<xsimd::batch<T, Arch>::size == N, xsimd::batch<T, Arch>,
                                                   typename sized_batch<T, N, xsimd::arch_list<Archs...>>::type>::type;
        };
    }

    /**
     * @brief type utility to select a batch of given type and size
     *
     * If one of the available architectures has a native vector type of the
     * given type and size, sets the @p type member to the appropriate batch
     * type. Otherwise set its to @p void.
     *
     * @tparam T the type of the underlying values.
     * @tparam N the number of elements of that type in the batch.
     **/
    template <typename T, std::size_t N>
    struct make_sized_batch
    {
        using type = typename details::sized_batch<T, N, supported_architectures>::type;
    };

    template <typename T, std::size_t N>
    using make_sized_batch_t = typename make_sized_batch<T, N>::type;
}

#endif
