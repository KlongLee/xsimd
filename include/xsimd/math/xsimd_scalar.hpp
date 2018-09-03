/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XSIMD_SCALAR_HPP
#define XSIMD_SCALAR_HPP

#include <cmath>

namespace xsimd
{

    /*********************************************
     * scalar fallback for xsimd math operations *
     *********************************************/

    using std::abs;
    using std::acos;
    using std::acosh;
    using std::asin;
    using std::asinh;
    using std::atan2;
    using std::atan;
    using std::atanh;
    using std::cbrt;
    using std::ceil;
    using std::copysign;
    using std::cos;
    using std::cosh;
    using std::erf;
    using std::erfc;
    using std::exp2;
    using std::exp;
    using std::expm1;
    using std::fdim;
    using std::fmax;
    using std::fmin;
    using std::floor;
    using std::fmod;
    using std::hypot;
    using std::isfinite;
    using std::isinf;
    using std::lgamma;
    using std::ldexp;
    using std::log10;
    using std::log1p;
    using std::log2;
    using std::log;
    using std::nearbyint;
    using std::proj;
    using std::remainder;
    using std::rint;
    using std::rint;
    using std::round;
    using std::round;
    using std::sin;
    using std::sinh;
    using std::sqrt;
    using std::tan;
    using std::tanh;
    using std::tgamma;
    using std::trunc;
    using std::trunc;

#ifdef XSIMD_ENABLE_XTL_COMPLEX
    using xtl::abs;
    using xtl::norm;
    using xtl::proj;
    using xtl::exp;
    using xtl::log;
    using xtl::log10;
    using xtl::pow;
    using xtl::sqrt;
    using xtl::sin;
    using xtl::cos;
    using xtl::tan;
    using xtl::asin;
    using xtl::acos;
    using xtl::atan;
    using xtl::sinh;
    using xtl::cosh;
    using xtl::tanh;
    using xtl::asinh;
    using xtl::acosh;
    using xtl::atanh;
#endif

    template <class T>
    inline bool is_flint(const T& x)
    {
        return std::isnan(x - x) ? std::numeric_limits<T>::quiet_NaN() : x - std::trunc(x);
    }

    template <class T>
    inline bool is_odd(const T& x)
    {
        return is_even(x - 1.);
    }

    template <class T>
    inline bool is_even(const T& x)
    {
        return is_flint(x * T(0.5));
    }

    template <class T>
    inline T exp10(const T& x)
    {
        // FIXME: faster alternatives exist
        return std::pow(T(10), x);
    }

    template <class T>
    inline std::complex<T> expm1(const std::complex<T>& val)
    {
        // FIXME: probably not accurate
        return std::exp(val) - 1;
    }

    template <class T0, class T1>
    inline typename std::enable_if<std::is_integral<T1>::value, T0>::type
    pow(const T0& t0, const T1& t1)
    {
      return detail::ipow(t0, t1);
    }

    template <class T0, class T1>
    inline typename std::enable_if<std::is_floating_point<T1>::value, T0>::type
    pow(const T0& t0, const T1& t1)
    {
        return std::pow(t0, t1);
    }

    template <class T0, class T1>
    inline typename std::enable_if<std::is_integral<T1>::value, std::complex<T0>>::type
    pow(const std::complex<T0>& t0, const T1& t1)
    {
      return detail::ipow(t0, t1);
    }

    template <class T0, class T1>
    inline typename std::enable_if<!std::is_integral<T1>::value, std::complex<T0>>::type
    pow(const std::complex<T0>& t0, const T1& t1)
    {
      return std::pow(t0, t1);
    }

    template <class T0, class T1>
    inline auto
    pow(const T0& t0, const std::complex<T1>& t1)
        -> typename std::enable_if<std::is_scalar<T0>::value, decltype(std::pow(t0, t1))>::type
    {
      return std::pow(t0, t1);
    }

    template <class T>
    inline auto bitofsign(T const& x) -> decltype(std::signbit(x))
    {
        return std::signbit(x);
    }

    template <class T>
    inline auto signbit(T const& v) -> decltype(bitofsign(v))
    {
        return bitofsign(v);
    }

    inline double sign(bool const &v)
    {
        return v;
    }

    template <class T>
    inline double sign(const T&v)
    {
        return v < 0 ? -1. : v == 0 ? 0. : 1.;
    }

    template <class T>
    inline std::complex<T> sign(const std::complex<T>& v)
    {
        if (v.real())
        {
            return {sign(v.real()), 0};
        }
        else
        {
            return {sign(v.imag()), 0};
        }
    }

    template <class T>
    std::complex<T> log2(const std::complex<T>& val)
    {
        return std::log(val) / std::log(T(2));
    }

    template <class T>
    inline std::complex<T> log1p(const std::complex<T>& val)
    {
        return std::log(val + 1);
    }


    template <class T0, class T1>
    inline auto min(T0 const &self, T1 const &other) ->
        typename std::enable_if<std::is_scalar<T0>::value && std::is_scalar<T1>::value,
                                typename std::decay<decltype(self > other ? other : self)>::type>::type
    {
        return self > other ? other : self;
    }

    // numpy defines minimum operator on complex using lexical comparison
    template <class T0, class T1>
    inline std::complex<typename std::common_type<T0, T1>::type>
    min(std::complex<T0> const &self, std::complex<T1> const &other)
    {
        return (self.real() < other.real()) ? (self) : (self.real() == other.real() ? (self.imag() < other.imag() ? self : other) : other);
    }

    template <class T0, class T1>
    inline auto max(T0 const &self, T1 const &other) ->
        typename std::enable_if<std::is_scalar<T0>::value && std::is_scalar<T1>::value,
                                typename std::decay<decltype(self > other ? other : self)>::type>::type
    {
        return self < other ? other : self;
    }

    // numpy defines maximum operator on complex using lexical comparison
    template <class T0, class T1>
    inline std::complex<typename std::common_type<T0, T1>::type>
    max(std::complex<T0> const &self, std::complex<T1> const &other)
    {
        return (self.real() > other.real()) ? (self) : (self.real() == other.real() ? (self.imag() > other.imag() ? self : other) : other);
    }

    template <class T>
    inline typename std::enable_if<std::is_scalar<T>::value, T>::type fma(const T& a, const T& b, const T& c)
    {
        return std::fma(a, b, c);
    }

    inline void sincos(float val, float&s, float& c)
    {
#ifdef _GNU_SOURCE
        ::sincosf(val, &s, &c);
#else
        s = std::sin(val);
        c = std::cos(val);
#endif
    }

    inline void sincos(double val, double&s, double& c)
    {
#ifdef _GNU_SOURCE
        ::sincos(val, &s, &c);
#else
        s = std::sin(val);
        c = std::cos(val);
#endif
    }

    template <class T>
    inline void sincos(const std::complex<T>& val, std::complex<T>& s, std::complex<T>& c)
    {
        s = std::sin(val);
        c = std::cos(val);
    }

#ifdef XSIMD_ENABLE_XTL_COMPLEX
    template <class T>
    inline void sincos(const xtl::xcomplex<T>& val, xtl::xcomplex<T>& s, xtl::xcomplex<T>& c)
    {
        s = sin(val);
        c = cos(val);
    }
#endif

    template <class T>
    inline T frexp(T const& val, int& exp)
    {
        return std::frexp(val, &exp);
    }

    template <class T>
    inline decltype(abs(std::declval<T>())) norm(const T& val)
    {
        auto tmp = abs(val);
        return tmp * tmp;
    }

}

#endif
