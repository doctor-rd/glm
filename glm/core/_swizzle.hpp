///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Mathematics (glm.g-truc.net)
///
/// Copyright (c) 2005 - 2011 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref core
/// @file glm/core/_swizzle.hpp
/// @date 2006-04-20 / 2011-02-16
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#ifndef glm_core_swizzle
#define glm_core_swizzle

namespace glm
{
	enum comp
	{
		X = 0,
		R = 0,
		S = 0,
		Y = 1,
		G = 1,
		T = 1,
		Z = 2,
		B = 2,
		P = 2,
		W = 3,
		A = 3,
		Q = 3
	};
}//namespace glm


namespace glm{
namespace detail
{
    //! Internal class for implementing swizzle operators
    /*!
        Template parameters:

        ValueType = type of scalar values (e.g. float, double)
        VecType   = class the swizzle is applies to (e.g. vector3f)
        N       = number of components in the vector (e.g. 3)
        E0...3  = what index the n-th element of this swizzle refers to
    */
    template <typename DerivedType, typename ValueType, typename VecType, int N, int E0, int E1, int E2, int E3, int DUPLICATE_ELEMENTS>
    struct swizzle_base
    {
        typedef DerivedType     derived_type;
        typedef VecType         vec_type;        
        typedef ValueType       value_type;

        swizzle_base& operator= (const VecType& that)
        {
            static const int offset_dst[4] = { E0, E1, E2, E3 };

            // Make a copy of the data in this == &that
            ValueType t[N];
            for (int i = 0; i < N; ++i)
                t[i] = that[i];
            for (int i = 0; i < N; ++i)
                elem(offset_dst[i]) = t[i];

            return *this;
        }

        swizzle_base& operator= (const ValueType& t)
        {
            static const int offset_dst[4] = { E0, E1, E2, E3 };

            for (int i = 0; i < N; ++i)
                elem(offset_dst[i]) = t;

            return *this;
        }

        void operator -= (const VecType& that)
        {
            static const int offset_dst[4] = { E0, E1, E2, E3 };

            ValueType t[N];
            for (int i = 0; i < N; ++i)
                t[i] = that[i];
            for (int i = 0; i < N; ++i)
                elem(offset_dst[i]) -= t[i];
        }

        void operator += (const VecType& that)
        {
            static const int offset_dst[4] = { E0, E1, E2, E3 };

            ValueType t[N];
            for (int i = 0; i < N; ++i)
                t[i] = that[i];
            for (int i = 0; i < N; ++i)
                elem(offset_dst[i]) += t[i];
        }

        void operator *= (const VecType& that)
        {
            static const int offset_dst[4] = { E0, E1, E2, E3 };

            ValueType t[N];
            for (int i = 0; i < N; ++i)
                t[i] = that[i];
            for (int i = 0; i < N; ++i)
                elem(offset_dst[i]) *= t[i];
        }

        void operator /= (const VecType& that)
        {
            static const int offset_dst[4] = { E0, E1, E2, E3 };

            ValueType t[N];
            for (int i = 0; i < N; ++i)
                t[i] = that[i];
            for (int i = 0; i < N; ++i)
                elem(offset_dst[i]) /= t[i];
        }

    protected:
        value_type&         elem   (size_t i)       { return (reinterpret_cast<value_type*>(_buffer))[i]; }
        const value_type&   elem   (size_t i) const { return (reinterpret_cast<const value_type*>(_buffer))[i]; }

        // Use an opaque buffer to *ensure* the compiler doesn't call a constructor.
        // Otherwise, a vec4 containing all swizzles might end up with 1000s of 
        // constructor calls
        char    _buffer[sizeof(value_type) * N];
    };

    template <typename DerivedType, typename ValueType, typename VecType, int N, int E0, int E1, int E2, int E3>
    struct swizzle_base<DerivedType,ValueType,VecType,N,E0,E1,E2,E3,1>
    {
        typedef DerivedType     derived_type;
        typedef VecType         vec_type;        
        typedef ValueType       value_type;

        struct Stub {};
        swizzle_base& operator= (const Stub& that) {}
          
    protected:
        value_type&         elem   (size_t i)       { return (reinterpret_cast<value_type*>(_buffer))[i]; }
        const value_type&   elem   (size_t i) const { return (reinterpret_cast<const value_type*>(_buffer))[i]; }

        char    _buffer[sizeof(value_type) * N];      
    };

    //! Internal class for implementing swizzle operators
    template <typename T, typename P, int E0, int E1>
    struct swizzle2 : public swizzle_base<swizzle2<T,P,E0,E1>,T,P,2,E0,E1,0,0,(E0 == E1)>
    {
        using swizzle_base<swizzle2<T,P,E0,E1>,T,P,2,E0,E1,0,0,(E0 == E1)>::operator=;
        P cast() const { return P(this->elem(E0), this->elem(E1)); }
        operator P () const { return cast(); }
    };

    //! Internal class for implementing swizzle operators
    template <typename T, typename P, int E0, int E1, int E2>
    struct swizzle2_3 : public swizzle_base<swizzle2_3<T,P,E0,E1,E2>,T,P,2,E0,E1,E2,0,1>
    {
        using swizzle_base<swizzle2_3<T,P,E0,E1,E2>,T,P,2,E0,E1,E2,0,1>::operator=;
        P cast() const { return P(this->elem(E0), this->elem(E1), this->elem(E2)); }
        operator P () const { return cast(); }
    };

    //! Internal class for implementing swizzle operators
    template <typename T, typename P, int E0, int E1, int E2, int E3>
    struct swizzle2_4 : public swizzle_base<swizzle2_4<T,P,E0,E1,E2,E3>,T,P,2,E0,E1,E2,E3,1>
    {
        using swizzle_base<swizzle2_4<T,P,E0,E1,E2,E3>,T,P,2,E0,E1,E2,E3,1>::operator=;
        P cast() const { return P(this->elem(E0), this->elem(E1), this->elem(E2), this->elem(E3)); }
        operator P () const { return cast(); }
    };

    //! Internal class for implementing swizzle operators
    template <typename T, typename P, int E0, int E1, int E2>
    struct swizzle3 : public swizzle_base<swizzle3<T,P,E0,E1,E2>,T,P,3,E0,E1,E2,0,(E0==E1||E0==E2||E1==E2)>
    {
        using swizzle_base<swizzle3<T,P,E0,E1,E2>,T,P,3,E0,E1,E2,0,(E0==E1||E0==E2||E1==E2)>::operator=;
        P cast() const { return P(this->elem(E0), this->elem(E1), this->elem(E2)); }
        operator P () const { return cast(); }
    };

    //! Internal class for implementing swizzle operators
    template <typename T, typename P, int E0, int E1>
    struct swizzle3_2 : public swizzle_base<swizzle3_2<T,P,E0,E1>,T,P,2,E0,E1,0,0,(E0==E1)>
    {
        using swizzle_base<swizzle3_2<T,P,E0,E1>,T,P,2,E0,E1,0,0,(E0==E1)>::operator=;
        P cast() const { return P(this->elem(E0), this->elem(E1)); }
        operator P () const { return cast(); }
    };

    //! Internal class for implementing swizzle operators
    template <typename T, typename P, int E0, int E1, int E2, int E3>
    struct swizzle3_4 : public swizzle_base<swizzle3_4<T,P,E0,E1,E2,E3>,T,P,3,E0,E1,E2,E3,1>
    {
        using swizzle_base<swizzle3_4<T,P,E0,E1,E2,E3>,T,P,3,E0,E1,E2,E3,1>::operator=;
        P cast() const { return P(this->elem(E0), this->elem(E1), this->elem(E2), this->elem(E3)); }
        operator P () const { return cast(); }
    };

    //! Internal class for implementing swizzle operators
    template <typename T, typename P, int E0, int E1, int E2, int E3>
    struct swizzle4 : public swizzle_base<swizzle4<T,P,E0,E1,E2,E3>,T,P,4,E0,E1,E2,E3,(E0==E1||E0==E2||E0==E3||E1==E2||E1==E3||E2==E3)>
    {
        using swizzle_base<swizzle4<T,P,E0,E1,E2,E3>,T,P,4,E0,E1,E2,E3,(E0==E1||E0==E2||E0==E3||E1==E2||E1==E3||E2==E3)>::operator=;
        P cast() const { return P(this->elem(E0), this->elem(E1), this->elem(E2), this->elem(E3)); }
        operator P () const { return cast(); }
    };

    //! Internal class for implementing swizzle operators
    template <typename T, typename P, int E0, int E1>
    struct swizzle4_2 : public swizzle_base<swizzle4_2<T,P,E0,E1>,T,P,2,E0,E1,0,0,(E0==E1)>
    {
        using swizzle_base<swizzle4_2<T,P,E0,E1>,T,P,2,E0,E1,0,0,(E0==E1)>::operator=;
        P cast() const { return P(this->elem(E0), this->elem(E1)); }
        operator P () const { return cast(); }
    };


    //! Internal class for implementing swizzle operators
    template <typename T, typename P, int E0, int E1, int E2>
    struct swizzle4_3 : public swizzle_base<swizzle4_3<T,P,E0,E1,E2>,T,P,4,E0,E1,E2,0,(E0==E1||E0==E2||E1==E2)>
    {
        using swizzle_base<swizzle4_3<T,P,E0,E1,E2>,T,P,4,E0,E1,E2,0,(E0==E1||E0==E2||E1==E2)>::operator=;
        P cast() const { return P(this->elem(E0), this->elem(E1), this->elem(E2)); }
        operator P () const { return cast(); }
    };

//
// To prevent the C++ syntax from getting *completely* overwhelming, define some alias macros
//
#define _GLM_SWIZZLE_TEMPLATE1   template <typename T, typename P, int N, typename S0, int E0, int E1, int E2, int E3, int D0>
#define _GLM_SWIZZLE_TEMPLATE2   template <typename T, typename P, int N, typename S0, int E0, int E1, int E2, int E3, int D0, typename S1,int F0, int F1, int F2, int F3, int D1>
#define _GLM_SWIZZLE_TYPE1       glm::detail::swizzle_base<S0,T,P,N,E0,E1,E2,E3,D0>
#define _GLM_SWIZZLE_TYPE2       glm::detail::swizzle_base<S1,T,P,N,F0,F1,F2,F3,D1>

#define _GLM_SWIZZLE_BINARY_OPERATOR_IMPLEMENTATION(OPERAND)\
    _GLM_SWIZZLE_TEMPLATE2 \
    typename P operator OPERAND ( const _GLM_SWIZZLE_TYPE1& a, const _GLM_SWIZZLE_TYPE2& b) \
    { \
        return static_cast<const S0&>(a).cast() OPERAND static_cast<const S1&>(b).cast(); \
    } \
    _GLM_SWIZZLE_TEMPLATE1 \
    typename P operator OPERAND ( const _GLM_SWIZZLE_TYPE1& a, const typename P& b) \
    { \
        return static_cast<const S0&>(a).cast() OPERAND b; \
    } \
    _GLM_SWIZZLE_TEMPLATE1 \
    typename P operator OPERAND ( const typename P& a, const _GLM_SWIZZLE_TYPE1& b) \
    { \
        return a OPERAND static_cast<const S0&>(b).cast(); \
    }

#define _GLM_SWIZZLE_SCALAR_BINARY_OPERATOR_IMPLEMENTATION(OPERAND)\
    _GLM_SWIZZLE_TEMPLATE1 \
    typename P operator OPERAND ( const _GLM_SWIZZLE_TYPE1& a, const typename T& b) \
    { \
        return static_cast<const S0&>(a).cast() OPERAND b; \
    } \
    _GLM_SWIZZLE_TEMPLATE1 \
    typename P operator OPERAND ( const typename T& a, const _GLM_SWIZZLE_TYPE1& b) \
    { \
        return a OPERAND static_cast<const S0&>(b).cast(); \
    }

#define _GLM_SWIZZLE_FUNCTION2(RETURN_TYPE,FUNCTION)\
    _GLM_SWIZZLE_TEMPLATE2\
    typename S0::RETURN_TYPE FUNCTION(const typename _GLM_SWIZZLE_TYPE1& a, const typename _GLM_SWIZZLE_TYPE2& b)\
    {\
        return FUNCTION(static_cast<const S0&>(a).cast(), static_cast<const S1&>(b).cast());\
    }\
    _GLM_SWIZZLE_TEMPLATE1\
    typename S0::RETURN_TYPE FUNCTION(const typename _GLM_SWIZZLE_TYPE1& a, const typename S0::vec_type& b)\
    {\
        return FUNCTION(static_cast<const S0&>(a).cast(), b);\
    }\
    _GLM_SWIZZLE_TEMPLATE1\
    typename S0::RETURN_TYPE FUNCTION(const typename S0::vec_type& a, const typename _GLM_SWIZZLE_TYPE1& b)\
    {\
        return FUNCTION(a, static_cast<const S0&>(b).cast());\
    }

    _GLM_SWIZZLE_TEMPLATE1  typename S0::vec_type   operator-   (typename S0::value_type a, const _GLM_SWIZZLE_TYPE1& b)        { return a - b; }

    _GLM_SWIZZLE_BINARY_OPERATOR_IMPLEMENTATION(+)
    _GLM_SWIZZLE_BINARY_OPERATOR_IMPLEMENTATION(-)
    _GLM_SWIZZLE_BINARY_OPERATOR_IMPLEMENTATION(*)
    _GLM_SWIZZLE_BINARY_OPERATOR_IMPLEMENTATION(/)

    _GLM_SWIZZLE_SCALAR_BINARY_OPERATOR_IMPLEMENTATION(*)

}//namespace detail 
}//namespace glm

namespace glm
{
        

    _GLM_SWIZZLE_FUNCTION2(value_type,  dot);
    _GLM_SWIZZLE_FUNCTION2(vec_type,    abs);
}


#define _GLM_SWIZZLE2_2_MEMBERS(T,P,E0,E1) \
    struct { glm::detail::swizzle2<T,P,0,0> E0 ## E0; }; \
    struct { glm::detail::swizzle2<T,P,0,1> E0 ## E1; }; \
    struct { glm::detail::swizzle2<T,P,1,0> E1 ## E0; }; \
    struct { glm::detail::swizzle2<T,P,1,1> E1 ## E1; }; 

#define _GLM_SWIZZLE2_3_MEMBERS(T,P2,E0,E1) \
    struct { glm::detail::swizzle2_3<T,P2,0,0,0> E0 ## E0 ## E0; }; \
    struct { glm::detail::swizzle2_3<T,P2,0,0,1> E0 ## E0 ## E1; }; \
    struct { glm::detail::swizzle2_3<T,P2,0,1,0> E0 ## E1 ## E0; }; \
    struct { glm::detail::swizzle2_3<T,P2,0,1,1> E0 ## E1 ## E1; }; \
    struct { glm::detail::swizzle2_3<T,P2,1,0,0> E1 ## E0 ## E0; }; \
    struct { glm::detail::swizzle2_3<T,P2,1,0,1> E1 ## E0 ## E1; }; \
    struct { glm::detail::swizzle2_3<T,P2,1,1,0> E1 ## E1 ## E0; }; \
    struct { glm::detail::swizzle2_3<T,P2,1,1,1> E1 ## E1 ## E1; };  


#define _GLM_SWIZZLE3_3_MEMBERS(T,P,E0,E1,E2) \
    struct { glm::detail::swizzle3<T,P,0,0,0> E0 ## E0 ## E0; }; \
    struct { glm::detail::swizzle3<T,P,0,0,1> E0 ## E0 ## E1; }; \
    struct { glm::detail::swizzle3<T,P,0,0,2> E0 ## E0 ## E2; }; \
    struct { glm::detail::swizzle3<T,P,0,1,0> E0 ## E1 ## E0; }; \
    struct { glm::detail::swizzle3<T,P,0,1,1> E0 ## E1 ## E1; }; \
    struct { glm::detail::swizzle3<T,P,0,1,2> E0 ## E1 ## E2; }; \
    struct { glm::detail::swizzle3<T,P,0,2,0> E0 ## E2 ## E0; }; \
    struct { glm::detail::swizzle3<T,P,0,2,1> E0 ## E2 ## E1; }; \
    struct { glm::detail::swizzle3<T,P,0,2,2> E0 ## E2 ## E2; }; \
    \
    struct { glm::detail::swizzle3<T,P,1,0,0> E1 ## E0 ## E0; }; \
    struct { glm::detail::swizzle3<T,P,1,0,1> E1 ## E0 ## E1; }; \
    struct { glm::detail::swizzle3<T,P,1,0,2> E1 ## E0 ## E2; }; \
    struct { glm::detail::swizzle3<T,P,1,1,0> E1 ## E1 ## E0; }; \
    struct { glm::detail::swizzle3<T,P,1,1,1> E1 ## E1 ## E1; }; \
    struct { glm::detail::swizzle3<T,P,1,1,2> E1 ## E1 ## E2; }; \
    struct { glm::detail::swizzle3<T,P,1,2,0> E1 ## E2 ## E0; }; \
    struct { glm::detail::swizzle3<T,P,1,2,1> E1 ## E2 ## E1; }; \
    struct { glm::detail::swizzle3<T,P,1,2,2> E1 ## E2 ## E2; }; \
    \
    struct { glm::detail::swizzle3<T,P,2,0,0> E2 ## E0 ## E0; }; \
    struct { glm::detail::swizzle3<T,P,2,0,1> E2 ## E0 ## E1; }; \
    struct { glm::detail::swizzle3<T,P,2,0,2> E2 ## E0 ## E2; }; \
    struct { glm::detail::swizzle3<T,P,2,1,0> E2 ## E1 ## E0; }; \
    struct { glm::detail::swizzle3<T,P,2,1,1> E2 ## E1 ## E1; }; \
    struct { glm::detail::swizzle3<T,P,2,1,2> E2 ## E1 ## E2; }; \
    struct { glm::detail::swizzle3<T,P,2,2,0> E2 ## E2 ## E0; }; \
    struct { glm::detail::swizzle3<T,P,2,2,1> E2 ## E2 ## E1; }; \
    struct { glm::detail::swizzle3<T,P,2,2,2> E2 ## E2 ## E2; };

#define _GLM_SWIZZLE3_2_MEMBERS(T,P2,E0,E1,E2) \
    struct { glm::detail::swizzle3_2<T,P2,0,0> E0 ## E0; }; \
    struct { glm::detail::swizzle3_2<T,P2,0,1> E0 ## E1; }; \
    struct { glm::detail::swizzle3_2<T,P2,0,2> E0 ## E2; }; \
    struct { glm::detail::swizzle3_2<T,P2,1,0> E1 ## E0; }; \
    struct { glm::detail::swizzle3_2<T,P2,1,1> E1 ## E1; }; \
    struct { glm::detail::swizzle3_2<T,P2,1,2> E1 ## E2; }; \
    struct { glm::detail::swizzle3_2<T,P2,2,0> E2 ## E0; }; \
    struct { glm::detail::swizzle3_2<T,P2,2,1> E2 ## E1; }; \
    struct { glm::detail::swizzle3_2<T,P2,2,2> E2 ## E2; }; 

#define _GLM_SWIZZLE3_4_MEMBERS(T,P2,E0,E1,E2) \
    struct { glm::detail::swizzle3_4<T,P2,0,0,0,0> E0 ## E0 ## E0 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,0,0,1> E0 ## E0 ## E0 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,0,0,2> E0 ## E0 ## E0 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,0,1,0> E0 ## E0 ## E1 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,0,1,1> E0 ## E0 ## E1 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,0,1,2> E0 ## E0 ## E1 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,0,2,0> E0 ## E0 ## E2 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,0,2,1> E0 ## E0 ## E2 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,0,2,2> E0 ## E0 ## E2 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,1,0,0> E0 ## E1 ## E0 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,1,0,1> E0 ## E1 ## E0 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,1,0,2> E0 ## E1 ## E0 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,1,1,0> E0 ## E1 ## E1 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,1,1,1> E0 ## E1 ## E1 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,1,1,2> E0 ## E1 ## E1 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,1,2,0> E0 ## E1 ## E2 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,1,2,1> E0 ## E1 ## E2 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,1,2,2> E0 ## E1 ## E2 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,2,0,0> E0 ## E2 ## E0 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,2,0,1> E0 ## E2 ## E0 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,2,0,2> E0 ## E2 ## E0 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,2,1,0> E0 ## E2 ## E1 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,2,1,1> E0 ## E2 ## E1 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,2,1,2> E0 ## E2 ## E1 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,2,2,0> E0 ## E2 ## E2 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,2,2,1> E0 ## E2 ## E2 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,0,2,2,2> E0 ## E2 ## E2 ## E2; }; \
    \
    struct { glm::detail::swizzle3_4<T,P2,1,0,0,0> E1 ## E0 ## E0 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,0,0,1> E1 ## E0 ## E0 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,0,0,2> E1 ## E0 ## E0 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,0,1,0> E1 ## E0 ## E1 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,0,1,1> E1 ## E0 ## E1 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,0,1,2> E1 ## E0 ## E1 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,0,2,0> E1 ## E0 ## E2 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,0,2,1> E1 ## E0 ## E2 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,0,2,2> E1 ## E0 ## E2 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,1,0,0> E1 ## E1 ## E0 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,1,0,1> E1 ## E1 ## E0 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,1,0,2> E1 ## E1 ## E0 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,1,1,0> E1 ## E1 ## E1 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,1,1,1> E1 ## E1 ## E1 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,1,1,2> E1 ## E1 ## E1 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,1,2,0> E1 ## E1 ## E2 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,1,2,1> E1 ## E1 ## E2 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,1,2,2> E1 ## E1 ## E2 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,2,0,0> E1 ## E2 ## E0 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,2,0,1> E1 ## E2 ## E0 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,2,0,2> E1 ## E2 ## E0 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,2,1,0> E1 ## E2 ## E1 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,2,1,1> E1 ## E2 ## E1 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,2,1,2> E1 ## E2 ## E1 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,2,2,0> E1 ## E2 ## E2 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,2,2,1> E1 ## E2 ## E2 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,1,2,2,2> E1 ## E2 ## E2 ## E2; }; \
    \
    struct { glm::detail::swizzle3_4<T,P2,2,0,0,0> E2 ## E0 ## E0 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,0,0,1> E2 ## E0 ## E0 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,0,0,2> E2 ## E0 ## E0 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,0,1,0> E2 ## E0 ## E1 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,0,1,1> E2 ## E0 ## E1 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,0,1,2> E2 ## E0 ## E1 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,0,2,0> E2 ## E0 ## E2 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,0,2,1> E2 ## E0 ## E2 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,0,2,2> E2 ## E0 ## E2 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,1,0,0> E2 ## E1 ## E0 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,1,0,1> E2 ## E1 ## E0 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,1,0,2> E2 ## E1 ## E0 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,1,1,0> E2 ## E1 ## E1 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,1,1,1> E2 ## E1 ## E1 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,1,1,2> E2 ## E1 ## E1 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,1,2,0> E2 ## E1 ## E2 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,1,2,1> E2 ## E1 ## E2 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,1,2,2> E2 ## E1 ## E2 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,2,0,0> E2 ## E2 ## E0 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,2,0,1> E2 ## E2 ## E0 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,2,0,2> E2 ## E2 ## E0 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,2,1,0> E2 ## E2 ## E1 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,2,1,1> E2 ## E2 ## E1 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,2,1,2> E2 ## E2 ## E1 ## E2; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,2,2,0> E2 ## E2 ## E2 ## E0; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,2,2,1> E2 ## E2 ## E2 ## E1; }; \
    struct { glm::detail::swizzle3_4<T,P2,2,2,2,2> E2 ## E2 ## E2 ## E2; }; \


#define _GLM_SWIZZLE2_4_MEMBERS(T,P2,E0,E1) \
    struct { glm::detail::swizzle2_4<T,P2,0,0,0,0> E0 ## E0 ## E0 ## E0; }; \
    struct { glm::detail::swizzle2_4<T,P2,0,0,0,1> E0 ## E0 ## E0 ## E1; }; \
    struct { glm::detail::swizzle2_4<T,P2,0,0,1,0> E0 ## E0 ## E1 ## E0; }; \
    struct { glm::detail::swizzle2_4<T,P2,0,0,1,1> E0 ## E0 ## E1 ## E1; }; \
    struct { glm::detail::swizzle2_4<T,P2,0,1,0,0> E0 ## E1 ## E0 ## E0; }; \
    struct { glm::detail::swizzle2_4<T,P2,0,1,0,1> E0 ## E1 ## E0 ## E1; }; \
    struct { glm::detail::swizzle2_4<T,P2,0,1,1,0> E0 ## E1 ## E1 ## E0; }; \
    struct { glm::detail::swizzle2_4<T,P2,0,1,1,1> E0 ## E1 ## E1 ## E1; }; \
    struct { glm::detail::swizzle2_4<T,P2,1,0,0,0> E1 ## E0 ## E0 ## E0; }; \
    struct { glm::detail::swizzle2_4<T,P2,1,0,0,1> E1 ## E0 ## E0 ## E1; }; \
    struct { glm::detail::swizzle2_4<T,P2,1,0,1,0> E1 ## E0 ## E1 ## E0; }; \
    struct { glm::detail::swizzle2_4<T,P2,1,0,1,1> E1 ## E0 ## E1 ## E1; }; \
    struct { glm::detail::swizzle2_4<T,P2,1,1,0,0> E1 ## E1 ## E0 ## E0; }; \
    struct { glm::detail::swizzle2_4<T,P2,1,1,0,1> E1 ## E1 ## E0 ## E1; }; \
    struct { glm::detail::swizzle2_4<T,P2,1,1,1,0> E1 ## E1 ## E1 ## E0; }; \
    struct { glm::detail::swizzle2_4<T,P2,1,1,1,1> E1 ## E1 ## E1 ## E1; };  


#define _GLM_SWIZZLE4_2_MEMBERS(T,P,E0,E1,E2,E3) \
    struct { glm::detail::swizzle4_2<T,P,0,0> E0 ## E0; }; \
    struct { glm::detail::swizzle4_2<T,P,0,1> E0 ## E1; }; \
    struct { glm::detail::swizzle4_2<T,P,0,2> E0 ## E2; }; \
    struct { glm::detail::swizzle4_2<T,P,0,3> E0 ## E3; }; \
    struct { glm::detail::swizzle4_2<T,P,1,0> E1 ## E0; }; \
    struct { glm::detail::swizzle4_2<T,P,1,1> E1 ## E1; }; \
    struct { glm::detail::swizzle4_2<T,P,1,2> E1 ## E2; }; \
    struct { glm::detail::swizzle4_2<T,P,1,3> E1 ## E3; }; \
    struct { glm::detail::swizzle4_2<T,P,2,0> E2 ## E0; }; \
    struct { glm::detail::swizzle4_2<T,P,2,1> E2 ## E1; }; \
    struct { glm::detail::swizzle4_2<T,P,2,2> E2 ## E2; }; \
    struct { glm::detail::swizzle4_2<T,P,2,3> E2 ## E3; }; \
    struct { glm::detail::swizzle4_2<T,P,3,0> E3 ## E0; }; \
    struct { glm::detail::swizzle4_2<T,P,3,1> E3 ## E1; }; \
    struct { glm::detail::swizzle4_2<T,P,3,2> E3 ## E2; }; \
    struct { glm::detail::swizzle4_2<T,P,3,3> E3 ## E3; }; 

#define _GLM_SWIZZLE4_3_MEMBERS(T,P,E0,E1,E2,E3) \
    struct { glm::detail::swizzle4_3<T,P,0,0,0> E0 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,0,0,1> E0 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,0,0,2> E0 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,0,0,3> E0 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4_3<T,P,0,1,0> E0 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,0,1,1> E0 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,0,1,2> E0 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,0,1,3> E0 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4_3<T,P,0,2,0> E0 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,0,2,1> E0 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,0,2,2> E0 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,0,2,3> E0 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4_3<T,P,0,3,0> E0 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,0,3,1> E0 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,0,3,2> E0 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,0,3,3> E0 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4_3<T,P,1,0,0> E1 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,1,0,1> E1 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,1,0,2> E1 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,1,0,3> E1 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4_3<T,P,1,1,0> E1 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,1,1,1> E1 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,1,1,2> E1 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,1,1,3> E1 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4_3<T,P,1,2,0> E1 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,1,2,1> E1 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,1,2,2> E1 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,1,2,3> E1 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4_3<T,P,1,3,0> E1 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,1,3,1> E1 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,1,3,2> E1 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,1,3,3> E1 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4_3<T,P,2,0,0> E2 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,2,0,1> E2 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,2,0,2> E2 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,2,0,3> E2 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4_3<T,P,2,1,0> E2 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,2,1,1> E2 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,2,1,2> E2 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,2,1,3> E2 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4_3<T,P,2,2,0> E2 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,2,2,1> E2 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,2,2,2> E2 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,2,2,3> E2 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4_3<T,P,2,3,0> E2 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,2,3,1> E2 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,2,3,2> E2 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,2,3,3> E2 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4_3<T,P,3,0,0> E3 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,3,0,1> E3 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,3,0,2> E3 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,3,0,3> E3 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4_3<T,P,3,1,0> E3 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,3,1,1> E3 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,3,1,2> E3 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,3,1,3> E3 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4_3<T,P,3,2,0> E3 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,3,2,1> E3 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,3,2,2> E3 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,3,2,3> E3 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4_3<T,P,3,3,0> E3 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4_3<T,P,3,3,1> E3 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4_3<T,P,3,3,2> E3 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4_3<T,P,3,3,3> E3 ## E3 ## E3; };  
    

#define _GLM_SWIZZLE4_4_MEMBERS(T,P,E0,E1,E2,E3) \
    struct { glm::detail::swizzle4<T,P,0,0,0,0> E0 ## E0 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,0,0,0,1> E0 ## E0 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,0,0,0,2> E0 ## E0 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,0,0,0,3> E0 ## E0 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,0,0,1,0> E0 ## E0 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,0,0,1,1> E0 ## E0 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,0,0,1,2> E0 ## E0 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,0,0,1,3> E0 ## E0 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,0,0,2,0> E0 ## E0 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,0,0,2,1> E0 ## E0 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,0,0,2,2> E0 ## E0 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,0,0,2,3> E0 ## E0 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,0,0,3,0> E0 ## E0 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,0,0,3,1> E0 ## E0 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,0,0,3,2> E0 ## E0 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,0,0,3,3> E0 ## E0 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4<T,P,0,1,0,0> E0 ## E1 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,0,1,0,1> E0 ## E1 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,0,1,0,2> E0 ## E1 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,0,1,0,3> E0 ## E1 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,0,1,1,0> E0 ## E1 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,0,1,1,1> E0 ## E1 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,0,1,1,2> E0 ## E1 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,0,1,1,3> E0 ## E1 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,0,1,2,0> E0 ## E1 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,0,1,2,1> E0 ## E1 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,0,1,2,2> E0 ## E1 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,0,1,2,3> E0 ## E1 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,0,1,3,0> E0 ## E1 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,0,1,3,1> E0 ## E1 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,0,1,3,2> E0 ## E1 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,0,1,3,3> E0 ## E1 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4<T,P,0,2,0,0> E0 ## E2 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,0,2,0,1> E0 ## E2 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,0,2,0,2> E0 ## E2 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,0,2,0,3> E0 ## E2 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,0,2,1,0> E0 ## E2 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,0,2,1,1> E0 ## E2 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,0,2,1,2> E0 ## E2 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,0,2,1,3> E0 ## E2 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,0,2,2,0> E0 ## E2 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,0,2,2,1> E0 ## E2 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,0,2,2,2> E0 ## E2 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,0,2,2,3> E0 ## E2 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,0,2,3,0> E0 ## E2 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,0,2,3,1> E0 ## E2 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,0,2,3,2> E0 ## E2 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,0,2,3,3> E0 ## E2 ## E3 ## E3; }; \
    \
    \
    struct { glm::detail::swizzle4<T,P,1,0,0,0> E1 ## E0 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,0,0,1> E1 ## E0 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,0,0,2> E1 ## E0 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,0,0,3> E1 ## E0 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,1,0,1,0> E1 ## E0 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,0,1,1> E1 ## E0 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,0,1,2> E1 ## E0 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,0,1,3> E1 ## E0 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,1,0,2,0> E1 ## E0 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,0,2,1> E1 ## E0 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,0,2,2> E1 ## E0 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,0,2,3> E1 ## E0 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,1,0,3,0> E1 ## E0 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,0,3,1> E1 ## E0 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,0,3,2> E1 ## E0 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,0,3,3> E1 ## E0 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4<T,P,1,1,0,0> E1 ## E1 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,1,0,1> E1 ## E1 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,1,0,2> E1 ## E1 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,1,0,3> E1 ## E1 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,1,1,1,0> E1 ## E1 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,1,1,1> E1 ## E1 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,1,1,2> E1 ## E1 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,1,1,3> E1 ## E1 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,1,1,2,0> E1 ## E1 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,1,2,1> E1 ## E1 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,1,2,2> E1 ## E1 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,1,2,3> E1 ## E1 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,1,1,3,0> E1 ## E1 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,1,3,1> E1 ## E1 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,1,3,2> E1 ## E1 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,1,3,3> E1 ## E1 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4<T,P,1,2,0,0> E1 ## E2 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,2,0,1> E1 ## E2 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,2,0,2> E1 ## E2 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,2,0,3> E1 ## E2 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,1,2,1,0> E1 ## E2 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,2,1,1> E1 ## E2 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,2,1,2> E1 ## E2 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,2,1,3> E1 ## E2 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,1,2,2,0> E1 ## E2 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,2,2,1> E1 ## E2 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,2,2,2> E1 ## E2 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,2,2,3> E1 ## E2 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,1,2,3,0> E1 ## E2 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,2,3,1> E1 ## E2 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,2,3,2> E1 ## E2 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,2,3,3> E1 ## E2 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4<T,P,1,3,0,0> E1 ## E3 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,3,0,1> E1 ## E3 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,3,0,2> E1 ## E3 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,3,0,3> E1 ## E3 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,1,3,1,0> E1 ## E3 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,3,1,1> E1 ## E3 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,3,1,2> E1 ## E3 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,3,1,3> E1 ## E3 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,1,3,2,0> E1 ## E3 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,3,2,1> E1 ## E3 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,3,2,2> E1 ## E3 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,3,2,3> E1 ## E3 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,1,3,3,0> E1 ## E3 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,1,3,3,1> E1 ## E3 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,1,3,3,2> E1 ## E3 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,1,3,3,3> E1 ## E3 ## E3 ## E3; }; \
    \
    \
    struct { glm::detail::swizzle4<T,P,2,0,0,0> E2 ## E0 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,0,0,1> E2 ## E0 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,0,0,2> E2 ## E0 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,0,0,3> E2 ## E0 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,2,0,1,0> E2 ## E0 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,0,1,1> E2 ## E0 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,0,1,2> E2 ## E0 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,0,1,3> E2 ## E0 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,2,0,2,0> E2 ## E0 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,0,2,1> E2 ## E0 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,0,2,2> E2 ## E0 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,0,2,3> E2 ## E0 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,2,0,3,0> E2 ## E0 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,0,3,1> E2 ## E0 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,0,3,2> E2 ## E0 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,0,3,3> E2 ## E0 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4<T,P,2,1,0,0> E2 ## E1 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,1,0,1> E2 ## E1 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,1,0,2> E2 ## E1 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,1,0,3> E2 ## E1 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,2,1,1,0> E2 ## E1 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,1,1,1> E2 ## E1 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,1,1,2> E2 ## E1 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,1,1,3> E2 ## E1 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,2,1,2,0> E2 ## E1 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,1,2,1> E2 ## E1 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,1,2,2> E2 ## E1 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,1,2,3> E2 ## E1 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,2,1,3,0> E2 ## E1 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,1,3,1> E2 ## E1 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,1,3,2> E2 ## E1 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,1,3,3> E2 ## E1 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4<T,P,2,2,0,0> E2 ## E2 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,2,0,1> E2 ## E2 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,2,0,2> E2 ## E2 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,2,0,3> E2 ## E2 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,2,2,1,0> E2 ## E2 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,2,1,1> E2 ## E2 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,2,1,2> E2 ## E2 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,2,1,3> E2 ## E2 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,2,2,2,0> E2 ## E2 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,2,2,1> E2 ## E2 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,2,2,2> E2 ## E2 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,2,2,3> E2 ## E2 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,2,2,3,0> E2 ## E2 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,2,3,1> E2 ## E2 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,2,3,2> E2 ## E2 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,2,3,3> E2 ## E2 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4<T,P,2,3,0,0> E2 ## E3 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,3,0,1> E2 ## E3 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,3,0,2> E2 ## E3 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,3,0,3> E2 ## E3 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,2,3,1,0> E2 ## E3 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,3,1,1> E2 ## E3 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,3,1,2> E2 ## E3 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,3,1,3> E2 ## E3 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,2,3,2,0> E2 ## E3 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,3,2,1> E2 ## E3 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,3,2,2> E2 ## E3 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,3,2,3> E2 ## E3 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,2,3,3,0> E2 ## E3 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,2,3,3,1> E2 ## E3 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,2,3,3,2> E2 ## E3 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,2,3,3,3> E2 ## E3 ## E3 ## E3; }; \
    \
    \
    struct { glm::detail::swizzle4<T,P,3,0,0,0> E3 ## E0 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,0,0,1> E3 ## E0 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,0,0,2> E3 ## E0 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,0,0,3> E3 ## E0 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,3,0,1,0> E3 ## E0 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,0,1,1> E3 ## E0 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,0,1,2> E3 ## E0 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,0,1,3> E3 ## E0 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,3,0,2,0> E3 ## E0 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,0,2,1> E3 ## E0 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,0,2,2> E3 ## E0 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,0,2,3> E3 ## E0 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,3,0,3,0> E3 ## E0 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,0,3,1> E3 ## E0 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,0,3,2> E3 ## E0 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,0,3,3> E3 ## E0 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4<T,P,3,1,0,0> E3 ## E1 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,1,0,1> E3 ## E1 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,1,0,2> E3 ## E1 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,1,0,3> E3 ## E1 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,3,1,1,0> E3 ## E1 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,1,1,1> E3 ## E1 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,1,1,2> E3 ## E1 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,1,1,3> E3 ## E1 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,3,1,2,0> E3 ## E1 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,1,2,1> E3 ## E1 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,1,2,2> E3 ## E1 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,1,2,3> E3 ## E1 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,3,1,3,0> E3 ## E1 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,1,3,1> E3 ## E1 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,1,3,2> E3 ## E1 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,1,3,3> E3 ## E1 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4<T,P,3,2,0,0> E3 ## E2 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,2,0,1> E3 ## E2 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,2,0,2> E3 ## E2 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,2,0,3> E3 ## E2 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,3,2,1,0> E3 ## E2 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,2,1,1> E3 ## E2 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,2,1,2> E3 ## E2 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,2,1,3> E3 ## E2 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,3,2,2,0> E3 ## E2 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,2,2,1> E3 ## E2 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,2,2,2> E3 ## E2 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,2,2,3> E3 ## E2 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,3,2,3,0> E3 ## E2 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,2,3,1> E3 ## E2 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,2,3,2> E3 ## E2 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,2,3,3> E3 ## E2 ## E3 ## E3; }; \
    \
    struct { glm::detail::swizzle4<T,P,3,3,0,0> E3 ## E3 ## E0 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,3,0,1> E3 ## E3 ## E0 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,3,0,2> E3 ## E3 ## E0 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,3,0,3> E3 ## E3 ## E0 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,3,3,1,0> E3 ## E3 ## E1 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,3,1,1> E3 ## E3 ## E1 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,3,1,2> E3 ## E3 ## E1 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,3,1,3> E3 ## E3 ## E1 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,3,3,2,0> E3 ## E3 ## E2 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,3,2,1> E3 ## E3 ## E2 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,3,2,2> E3 ## E3 ## E2 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,3,2,3> E3 ## E3 ## E2 ## E3; }; \
    struct { glm::detail::swizzle4<T,P,3,3,3,0> E3 ## E3 ## E3 ## E0; }; \
    struct { glm::detail::swizzle4<T,P,3,3,3,1> E3 ## E3 ## E3 ## E1; }; \
    struct { glm::detail::swizzle4<T,P,3,3,3,2> E3 ## E3 ## E3 ## E2; }; \
    struct { glm::detail::swizzle4<T,P,3,3,3,3> E3 ## E3 ## E3 ## E3; }; 





#if(defined(GLM_SWIZZLE_XYZW) || defined(GLM_SWIZZLE))

#define xx swizzle(glm::X, glm::X)
#define yx swizzle(glm::Y, glm::X)
#define zx swizzle(glm::Z, glm::X)
#define wx swizzle(glm::W, glm::X)
#define xy swizzle(glm::X, glm::Y)
#define yy swizzle(glm::Y, glm::Y)
#define zy swizzle(glm::Z, glm::Y)
#define wy swizzle(glm::W, glm::Y)
#define xz swizzle(glm::X, glm::Z)
#define yz swizzle(glm::Y, glm::Z)
#define zz swizzle(glm::Z, glm::Z)
#define wz swizzle(glm::W, glm::Z)
#define xw swizzle(glm::X, glm::W)
#define yw swizzle(glm::Y, glm::W)
#define zw swizzle(glm::Z, glm::W)
#define ww swizzle(glm::W, glm::W)

#endif

#if(defined(GLM_SWIZZLE_RGBA) || defined(GLM_SWIZZLE))

#define rr swizzle(glm::X, glm::X)
#define gr swizzle(glm::Y, glm::X)
#define br swizzle(glm::Z, glm::X)
#define ar swizzle(glm::W, glm::X)
#define rg swizzle(glm::X, glm::Y)
#define gg swizzle(glm::Y, glm::Y)
#define bg swizzle(glm::Z, glm::Y)
#define ag swizzle(glm::W, glm::Y)
#define rb swizzle(glm::X, glm::Z)
#define gb swizzle(glm::Y, glm::Z)
#define bb swizzle(glm::Z, glm::Z)
#define ab swizzle(glm::W, glm::Z)
#define ra swizzle(glm::X, glm::W)
#define ga swizzle(glm::Y, glm::W)
#define ba swizzle(glm::Z, glm::W)
#define aa swizzle(glm::W, glm::W)

#endif

#if(defined(GLM_FORCE_SWIZZLE_STPQ) || defined(GLM_SWIZZLE))

#define ss swizzle(glm::X, glm::X)
#define ts swizzle(glm::Y, glm::X)
#define ps swizzle(glm::Z, glm::X)
#define qs swizzle(glm::W, glm::X)
#define st swizzle(glm::X, glm::Y)
#define tt swizzle(glm::Y, glm::Y)
#define pt swizzle(glm::Z, glm::Y)
#define qt swizzle(glm::W, glm::Y)
#define sp swizzle(glm::X, glm::Z)
#define tp swizzle(glm::Y, glm::Z)
#define pp swizzle(glm::Z, glm::Z)
#define qp swizzle(glm::W, glm::Z)
#define sq swizzle(glm::X, glm::W)
#define tq swizzle(glm::Y, glm::W)
#define pq swizzle(glm::Z, glm::W)
#define qq swizzle(glm::W, glm::W)

#endif

#if(defined(GLM_SWIZZLE_XYZW) || defined(GLM_SWIZZLE))

#define xxx swizzle(glm::X, glm::X, glm::X)
#define yxx swizzle(glm::Y, glm::X, glm::X)
#define zxx swizzle(glm::Z, glm::X, glm::X)
#define wxx swizzle(glm::W, glm::X, glm::X)
#define xyx swizzle(glm::X, glm::Y, glm::X)
#define yyx swizzle(glm::Y, glm::Y, glm::X)
#define zyx swizzle(glm::Z, glm::Y, glm::X)
#define wyx swizzle(glm::W, glm::Y, glm::X)
#define xzx swizzle(glm::X, glm::Z, glm::X)
#define yzx swizzle(glm::Y, glm::Z, glm::X)
#define zzx swizzle(glm::Z, glm::Z, glm::X)
#define wzx swizzle(glm::W, glm::Z, glm::X)
#define xwx swizzle(glm::X, glm::W, glm::X)
#define ywx swizzle(glm::Y, glm::W, glm::X)
#define zwx swizzle(glm::Z, glm::W, glm::X)
#define wwx swizzle(glm::W, glm::W, glm::X)
#define xxy swizzle(glm::X, glm::X, glm::Y)
#define yxy swizzle(glm::Y, glm::X, glm::Y)
#define zxy swizzle(glm::Z, glm::X, glm::Y)
#define wxy swizzle(glm::W, glm::X, glm::Y)
#define xyy swizzle(glm::X, glm::Y, glm::Y)
#define yyy swizzle(glm::Y, glm::Y, glm::Y)
#define zyy swizzle(glm::Z, glm::Y, glm::Y)
#define wyy swizzle(glm::W, glm::Y, glm::Y)
#define xzy swizzle(glm::X, glm::Z, glm::Y)
#define yzy swizzle(glm::Y, glm::Z, glm::Y)
#define zzy swizzle(glm::Z, glm::Z, glm::Y)
#define wzy swizzle(glm::W, glm::Z, glm::Y)
#define xwy swizzle(glm::X, glm::W, glm::Y)
#define ywy swizzle(glm::Y, glm::W, glm::Y)
#define zwy swizzle(glm::Z, glm::W, glm::Y)
#define wwy swizzle(glm::W, glm::W, glm::Y)
#define xxz swizzle(glm::X, glm::X, glm::Z)
#define yxz swizzle(glm::Y, glm::X, glm::Z)
#define zxz swizzle(glm::Z, glm::X, glm::Z)
#define wxz swizzle(glm::W, glm::X, glm::Z)
#define xyz swizzle(glm::X, glm::Y, glm::Z)
#define yyz swizzle(glm::Y, glm::Y, glm::Z)
#define zyz swizzle(glm::Z, glm::Y, glm::Z)
#define wyz swizzle(glm::W, glm::Y, glm::Z)
#define xzz swizzle(glm::X, glm::Z, glm::Z)
#define yzz swizzle(glm::Y, glm::Z, glm::Z)
#define zzz swizzle(glm::Z, glm::Z, glm::Z)
#define wzz swizzle(glm::W, glm::Z, glm::Z)
#define xwz swizzle(glm::X, glm::W, glm::Z)
#define ywz swizzle(glm::Y, glm::W, glm::Z)
#define zwz swizzle(glm::Z, glm::W, glm::Z)
#define wwz swizzle(glm::W, glm::W, glm::Z)
#define xxw swizzle(glm::X, glm::X, glm::W)
#define yxw swizzle(glm::Y, glm::X, glm::W)
#define zxw swizzle(glm::Z, glm::X, glm::W)
#define wxw swizzle(glm::W, glm::X, glm::W)
#define xyw swizzle(glm::X, glm::Y, glm::W)
#define yyw swizzle(glm::Y, glm::Y, glm::W)
#define zyw swizzle(glm::Z, glm::Y, glm::W)
#define wyw swizzle(glm::W, glm::Y, glm::W)
#define xzw swizzle(glm::X, glm::Z, glm::W)
#define yzw swizzle(glm::Y, glm::Z, glm::W)
#define zzw swizzle(glm::Z, glm::Z, glm::W)
#define wzw swizzle(glm::W, glm::Z, glm::W)
#define xww swizzle(glm::X, glm::W, glm::W)
#define yww swizzle(glm::Y, glm::W, glm::W)
#define zww swizzle(glm::Z, glm::W, glm::W)
#define www swizzle(glm::W, glm::W, glm::W)

#endif

#if(defined(GLM_SWIZZLE_RGBA) || defined(GLM_SWIZZLE))

#define rrr swizzle(glm::X, glm::X, glm::X)
#define grr swizzle(glm::Y, glm::X, glm::X)
#define brr swizzle(glm::Z, glm::X, glm::X)
#define arr swizzle(glm::W, glm::X, glm::X)
#define rgr swizzle(glm::X, glm::Y, glm::X)
#define ggr swizzle(glm::Y, glm::Y, glm::X)
#define bgr swizzle(glm::Z, glm::Y, glm::X)
#define agr swizzle(glm::W, glm::Y, glm::X)
#define rbr swizzle(glm::X, glm::Z, glm::X)
#define gbr swizzle(glm::Y, glm::Z, glm::X)
#define bbr swizzle(glm::Z, glm::Z, glm::X)
#define abr swizzle(glm::W, glm::Z, glm::X)
#define rar swizzle(glm::X, glm::W, glm::X)
#define gar swizzle(glm::Y, glm::W, glm::X)
#define bar swizzle(glm::Z, glm::W, glm::X)
#define aar swizzle(glm::W, glm::W, glm::X)
#define rrg swizzle(glm::X, glm::X, glm::Y)
#define grg swizzle(glm::Y, glm::X, glm::Y)
#define brg swizzle(glm::Z, glm::X, glm::Y)
#define arg swizzle(glm::W, glm::X, glm::Y)
#define rgg swizzle(glm::X, glm::Y, glm::Y)
#define ggg swizzle(glm::Y, glm::Y, glm::Y)
#define bgg swizzle(glm::Z, glm::Y, glm::Y)
#define agg swizzle(glm::W, glm::Y, glm::Y)
#define rbg swizzle(glm::X, glm::Z, glm::Y)
#define gbg swizzle(glm::Y, glm::Z, glm::Y)
#define bbg swizzle(glm::Z, glm::Z, glm::Y)
#define abg swizzle(glm::W, glm::Z, glm::Y)
#define rag swizzle(glm::X, glm::W, glm::Y)
#define gag swizzle(glm::Y, glm::W, glm::Y)
#define bag swizzle(glm::Z, glm::W, glm::Y)
#define aag swizzle(glm::W, glm::W, glm::Y)
#define rrb swizzle(glm::X, glm::X, glm::Z)
#define grb swizzle(glm::Y, glm::X, glm::Z)
#define brb swizzle(glm::Z, glm::X, glm::Z)
#define arb swizzle(glm::W, glm::X, glm::Z)
#define rgb swizzle(glm::X, glm::Y, glm::Z)
#define ggb swizzle(glm::Y, glm::Y, glm::Z)
#define bgb swizzle(glm::Z, glm::Y, glm::Z)
#define agb swizzle(glm::W, glm::Y, glm::Z)
#define rbb swizzle(glm::X, glm::Z, glm::Z)
#define gbb swizzle(glm::Y, glm::Z, glm::Z)
#define bbb swizzle(glm::Z, glm::Z, glm::Z)
#define abb swizzle(glm::W, glm::Z, glm::Z)
#define rab swizzle(glm::X, glm::W, glm::Z)
#define gab swizzle(glm::Y, glm::W, glm::Z)
#define bab swizzle(glm::Z, glm::W, glm::Z)
#define aab swizzle(glm::W, glm::W, glm::Z)
#define rra swizzle(glm::X, glm::X, glm::W)
#define gra swizzle(glm::Y, glm::X, glm::W)
#define bra swizzle(glm::Z, glm::X, glm::W)
#define ara swizzle(glm::W, glm::X, glm::W)
#define rga swizzle(glm::X, glm::Y, glm::W)
#define gga swizzle(glm::Y, glm::Y, glm::W)
#define bga swizzle(glm::Z, glm::Y, glm::W)
#define aga swizzle(glm::W, glm::Y, glm::W)
#define rba swizzle(glm::X, glm::Z, glm::W)
#define gba swizzle(glm::Y, glm::Z, glm::W)
#define bba swizzle(glm::Z, glm::Z, glm::W)
#define aba swizzle(glm::W, glm::Z, glm::W)
#define raa swizzle(glm::X, glm::W, glm::W)
#define gaa swizzle(glm::Y, glm::W, glm::W)
#define baa swizzle(glm::Z, glm::W, glm::W)
#define aaa swizzle(glm::W, glm::W, glm::W)

#endif

#if(defined(GLM_FORCE_SWIZZLE_STPQ) || defined(GLM_SWIZZLE))

#define sss swizzle(glm::X, glm::X, glm::X)
#define tss swizzle(glm::Y, glm::X, glm::X)
#define pss swizzle(glm::Z, glm::X, glm::X)
#define qss swizzle(glm::W, glm::X, glm::X)
#define sts swizzle(glm::X, glm::Y, glm::X)
#define tts swizzle(glm::Y, glm::Y, glm::X)
#define pts swizzle(glm::Z, glm::Y, glm::X)
#define qts swizzle(glm::W, glm::Y, glm::X)
#define sps swizzle(glm::X, glm::Z, glm::X)
#define tps swizzle(glm::Y, glm::Z, glm::X)
#define pps swizzle(glm::Z, glm::Z, glm::X)
#define qps swizzle(glm::W, glm::Z, glm::X)
#define sqs swizzle(glm::X, glm::W, glm::X)
#define tqs swizzle(glm::Y, glm::W, glm::X)
#define pqs swizzle(glm::Z, glm::W, glm::X)
#define qqs swizzle(glm::W, glm::W, glm::X)
#define sst swizzle(glm::X, glm::X, glm::Y)
#define tst swizzle(glm::Y, glm::X, glm::Y)
#define pst swizzle(glm::Z, glm::X, glm::Y)
#define qst swizzle(glm::W, glm::X, glm::Y)
#define stt swizzle(glm::X, glm::Y, glm::Y)
#define ttt swizzle(glm::Y, glm::Y, glm::Y)
#define ptt swizzle(glm::Z, glm::Y, glm::Y)
#define qtt swizzle(glm::W, glm::Y, glm::Y)
#define spt swizzle(glm::X, glm::Z, glm::Y)
#define tpt swizzle(glm::Y, glm::Z, glm::Y)
#define ppt swizzle(glm::Z, glm::Z, glm::Y)
#define qpt swizzle(glm::W, glm::Z, glm::Y)
#define sqt swizzle(glm::X, glm::W, glm::Y)
#define tqt swizzle(glm::Y, glm::W, glm::Y)
#define pqt swizzle(glm::Z, glm::W, glm::Y)
#define qqt swizzle(glm::W, glm::W, glm::Y)
#define ssp swizzle(glm::X, glm::X, glm::Z)
#define tsp swizzle(glm::Y, glm::X, glm::Z)
#define psp swizzle(glm::Z, glm::X, glm::Z)
#define qsp swizzle(glm::W, glm::X, glm::Z)
#define stp swizzle(glm::X, glm::Y, glm::Z)
#define ttp swizzle(glm::Y, glm::Y, glm::Z)
#define ptp swizzle(glm::Z, glm::Y, glm::Z)
#define qtp swizzle(glm::W, glm::Y, glm::Z)
#define spp swizzle(glm::X, glm::Z, glm::Z)
#define tpp swizzle(glm::Y, glm::Z, glm::Z)
#define ppp swizzle(glm::Z, glm::Z, glm::Z)
#define qpp swizzle(glm::W, glm::Z, glm::Z)
#define sqp swizzle(glm::X, glm::W, glm::Z)
#define tqp swizzle(glm::Y, glm::W, glm::Z)
#define pqp swizzle(glm::Z, glm::W, glm::Z)
#define qqp swizzle(glm::W, glm::W, glm::Z)
#define ssq swizzle(glm::X, glm::X, glm::W)
#define tsq swizzle(glm::Y, glm::X, glm::W)
#define psq swizzle(glm::Z, glm::X, glm::W)
#define qsq swizzle(glm::W, glm::X, glm::W)
#define stq swizzle(glm::X, glm::Y, glm::W)
#define ttq swizzle(glm::Y, glm::Y, glm::W)
#define ptq swizzle(glm::Z, glm::Y, glm::W)
#define qtq swizzle(glm::W, glm::Y, glm::W)
#define spq swizzle(glm::X, glm::Z, glm::W)
#define tpq swizzle(glm::Y, glm::Z, glm::W)
#define ppq swizzle(glm::Z, glm::Z, glm::W)
#define qpq swizzle(glm::W, glm::Z, glm::W)
#define sqq swizzle(glm::X, glm::W, glm::W)
#define tqq swizzle(glm::Y, glm::W, glm::W)
#define pqq swizzle(glm::Z, glm::W, glm::W)
#define qqq swizzle(glm::W, glm::W, glm::W)

#endif

#if(defined(GLM_SWIZZLE_XYZW) || defined(GLM_SWIZZLE))

#define xxxx swizzle(glm::X, glm::X, glm::X, glm::X)
#define yxxx swizzle(glm::Y, glm::X, glm::X, glm::X)
#define zxxx swizzle(glm::Z, glm::X, glm::X, glm::X)
#define wxxx swizzle(glm::W, glm::X, glm::X, glm::X)
#define xyxx swizzle(glm::X, glm::Y, glm::X, glm::X)
#define yyxx swizzle(glm::Y, glm::Y, glm::X, glm::X)
#define zyxx swizzle(glm::Z, glm::Y, glm::X, glm::X)
#define wyxx swizzle(glm::W, glm::Y, glm::X, glm::X)
#define xzxx swizzle(glm::X, glm::Z, glm::X, glm::X)
#define yzxx swizzle(glm::Y, glm::Z, glm::X, glm::X)
#define zzxx swizzle(glm::Z, glm::Z, glm::X, glm::X)
#define wzxx swizzle(glm::W, glm::Z, glm::X, glm::X)
#define xwxx swizzle(glm::X, glm::W, glm::X, glm::X)
#define ywxx swizzle(glm::Y, glm::W, glm::X, glm::X)
#define zwxx swizzle(glm::Z, glm::W, glm::X, glm::X)
#define wwxx swizzle(glm::W, glm::W, glm::X, glm::X)
#define xxyx swizzle(glm::X, glm::X, glm::Y, glm::X)
#define yxyx swizzle(glm::Y, glm::X, glm::Y, glm::X)
#define zxyx swizzle(glm::Z, glm::X, glm::Y, glm::X)
#define wxyx swizzle(glm::W, glm::X, glm::Y, glm::X)
#define xyyx swizzle(glm::X, glm::Y, glm::Y, glm::X)
#define yyyx swizzle(glm::Y, glm::Y, glm::Y, glm::X)
#define zyyx swizzle(glm::Z, glm::Y, glm::Y, glm::X)
#define wyyx swizzle(glm::W, glm::Y, glm::Y, glm::X)
#define xzyx swizzle(glm::X, glm::Z, glm::Y, glm::X)
#define yzyx swizzle(glm::Y, glm::Z, glm::Y, glm::X)
#define zzyx swizzle(glm::Z, glm::Z, glm::Y, glm::X)
#define wzyx swizzle(glm::W, glm::Z, glm::Y, glm::X)
#define xwyx swizzle(glm::X, glm::W, glm::Y, glm::X)
#define ywyx swizzle(glm::Y, glm::W, glm::Y, glm::X)
#define zwyx swizzle(glm::Z, glm::W, glm::Y, glm::X)
#define wwyx swizzle(glm::W, glm::W, glm::Y, glm::X)
#define xxzx swizzle(glm::X, glm::X, glm::Z, glm::X)
#define yxzx swizzle(glm::Y, glm::X, glm::Z, glm::X)
#define zxzx swizzle(glm::Z, glm::X, glm::Z, glm::X)
#define wxzx swizzle(glm::W, glm::X, glm::Z, glm::X)
#define xyzx swizzle(glm::X, glm::Y, glm::Z, glm::X)
#define yyzx swizzle(glm::Y, glm::Y, glm::Z, glm::X)
#define zyzx swizzle(glm::Z, glm::Y, glm::Z, glm::X)
#define wyzx swizzle(glm::W, glm::Y, glm::Z, glm::X)
#define xzzx swizzle(glm::X, glm::Z, glm::Z, glm::X)
#define yzzx swizzle(glm::Y, glm::Z, glm::Z, glm::X)
#define zzzx swizzle(glm::Z, glm::Z, glm::Z, glm::X)
#define wzzx swizzle(glm::W, glm::Z, glm::Z, glm::X)
#define xwzx swizzle(glm::X, glm::W, glm::Z, glm::X)
#define ywzx swizzle(glm::Y, glm::W, glm::Z, glm::X)
#define zwzx swizzle(glm::Z, glm::W, glm::Z, glm::X)
#define wwzx swizzle(glm::W, glm::W, glm::Z, glm::X)
#define xxwx swizzle(glm::X, glm::X, glm::W, glm::X)
#define yxwx swizzle(glm::Y, glm::X, glm::W, glm::X)
#define zxwx swizzle(glm::Z, glm::X, glm::W, glm::X)
#define wxwx swizzle(glm::W, glm::X, glm::W, glm::X)
#define xywx swizzle(glm::X, glm::Y, glm::W, glm::X)
#define yywx swizzle(glm::Y, glm::Y, glm::W, glm::X)
#define zywx swizzle(glm::Z, glm::Y, glm::W, glm::X)
#define wywx swizzle(glm::W, glm::Y, glm::W, glm::X)
#define xzwx swizzle(glm::X, glm::Z, glm::W, glm::X)
#define yzwx swizzle(glm::Y, glm::Z, glm::W, glm::X)
#define zzwx swizzle(glm::Z, glm::Z, glm::W, glm::X)
#define wzwx swizzle(glm::W, glm::Z, glm::W, glm::X)
#define xwwx swizzle(glm::X, glm::W, glm::W, glm::X)
#define ywwx swizzle(glm::Y, glm::W, glm::W, glm::X)
#define zwwx swizzle(glm::Z, glm::W, glm::W, glm::X)
#define wwwx swizzle(glm::W, glm::W, glm::W, glm::X)
#define xxxy swizzle(glm::X, glm::X, glm::X, glm::Y)
#define yxxy swizzle(glm::Y, glm::X, glm::X, glm::Y)
#define zxxy swizzle(glm::Z, glm::X, glm::X, glm::Y)
#define wxxy swizzle(glm::W, glm::X, glm::X, glm::Y)
#define xyxy swizzle(glm::X, glm::Y, glm::X, glm::Y)
#define yyxy swizzle(glm::Y, glm::Y, glm::X, glm::Y)
#define zyxy swizzle(glm::Z, glm::Y, glm::X, glm::Y)
#define wyxy swizzle(glm::W, glm::Y, glm::X, glm::Y)
#define xzxy swizzle(glm::X, glm::Z, glm::X, glm::Y)
#define yzxy swizzle(glm::Y, glm::Z, glm::X, glm::Y)
#define zzxy swizzle(glm::Z, glm::Z, glm::X, glm::Y)
#define wzxy swizzle(glm::W, glm::Z, glm::X, glm::Y)
#define xwxy swizzle(glm::X, glm::W, glm::X, glm::Y)
#define ywxy swizzle(glm::Y, glm::W, glm::X, glm::Y)
#define zwxy swizzle(glm::Z, glm::W, glm::X, glm::Y)
#define wwxy swizzle(glm::W, glm::W, glm::X, glm::Y)
#define xxyy swizzle(glm::X, glm::X, glm::Y, glm::Y)
#define yxyy swizzle(glm::Y, glm::X, glm::Y, glm::Y)
#define zxyy swizzle(glm::Z, glm::X, glm::Y, glm::Y)
#define wxyy swizzle(glm::W, glm::X, glm::Y, glm::Y)
#define xyyy swizzle(glm::X, glm::Y, glm::Y, glm::Y)
#define yyyy swizzle(glm::Y, glm::Y, glm::Y, glm::Y)
#define zyyy swizzle(glm::Z, glm::Y, glm::Y, glm::Y)
#define wyyy swizzle(glm::W, glm::Y, glm::Y, glm::Y)
#define xzyy swizzle(glm::X, glm::Z, glm::Y, glm::Y)
#define yzyy swizzle(glm::Y, glm::Z, glm::Y, glm::Y)
#define zzyy swizzle(glm::Z, glm::Z, glm::Y, glm::Y)
#define wzyy swizzle(glm::W, glm::Z, glm::Y, glm::Y)
#define xwyy swizzle(glm::X, glm::W, glm::Y, glm::Y)
#define ywyy swizzle(glm::Y, glm::W, glm::Y, glm::Y)
#define zwyy swizzle(glm::Z, glm::W, glm::Y, glm::Y)
#define wwyy swizzle(glm::W, glm::W, glm::Y, glm::Y)
#define xxzy swizzle(glm::X, glm::X, glm::Z, glm::Y)
#define yxzy swizzle(glm::Y, glm::X, glm::Z, glm::Y)
#define zxzy swizzle(glm::Z, glm::X, glm::Z, glm::Y)
#define wxzy swizzle(glm::W, glm::X, glm::Z, glm::Y)
#define xyzy swizzle(glm::X, glm::Y, glm::Z, glm::Y)
#define yyzy swizzle(glm::Y, glm::Y, glm::Z, glm::Y)
#define zyzy swizzle(glm::Z, glm::Y, glm::Z, glm::Y)
#define wyzy swizzle(glm::W, glm::Y, glm::Z, glm::Y)
#define xzzy swizzle(glm::X, glm::Z, glm::Z, glm::Y)
#define yzzy swizzle(glm::Y, glm::Z, glm::Z, glm::Y)
#define zzzy swizzle(glm::Z, glm::Z, glm::Z, glm::Y)
#define wzzy swizzle(glm::W, glm::Z, glm::Z, glm::Y)
#define xwzy swizzle(glm::X, glm::W, glm::Z, glm::Y)
#define ywzy swizzle(glm::Y, glm::W, glm::Z, glm::Y)
#define zwzy swizzle(glm::Z, glm::W, glm::Z, glm::Y)
#define wwzy swizzle(glm::W, glm::W, glm::Z, glm::Y)
#define xxwy swizzle(glm::X, glm::X, glm::W, glm::Y)
#define yxwy swizzle(glm::Y, glm::X, glm::W, glm::Y)
#define zxwy swizzle(glm::Z, glm::X, glm::W, glm::Y)
#define wxwy swizzle(glm::W, glm::X, glm::W, glm::Y)
#define xywy swizzle(glm::X, glm::Y, glm::W, glm::Y)
#define yywy swizzle(glm::Y, glm::Y, glm::W, glm::Y)
#define zywy swizzle(glm::Z, glm::Y, glm::W, glm::Y)
#define wywy swizzle(glm::W, glm::Y, glm::W, glm::Y)
#define xzwy swizzle(glm::X, glm::Z, glm::W, glm::Y)
#define yzwy swizzle(glm::Y, glm::Z, glm::W, glm::Y)
#define zzwy swizzle(glm::Z, glm::Z, glm::W, glm::Y)
#define wzwy swizzle(glm::W, glm::Z, glm::W, glm::Y)
#define xwwy swizzle(glm::X, glm::W, glm::W, glm::Y)
#define ywwy swizzle(glm::Y, glm::W, glm::W, glm::Y)
#define zwwy swizzle(glm::Z, glm::W, glm::W, glm::Y)
#define wwwy swizzle(glm::W, glm::W, glm::W, glm::Y)
#define xxxz swizzle(glm::X, glm::X, glm::X, glm::Z)
#define yxxz swizzle(glm::Y, glm::X, glm::X, glm::Z)
#define zxxz swizzle(glm::Z, glm::X, glm::X, glm::Z)
#define wxxz swizzle(glm::W, glm::X, glm::X, glm::Z)
#define xyxz swizzle(glm::X, glm::Y, glm::X, glm::Z)
#define yyxz swizzle(glm::Y, glm::Y, glm::X, glm::Z)
#define zyxz swizzle(glm::Z, glm::Y, glm::X, glm::Z)
#define wyxz swizzle(glm::W, glm::Y, glm::X, glm::Z)
#define xzxz swizzle(glm::X, glm::Z, glm::X, glm::Z)
#define yzxz swizzle(glm::Y, glm::Z, glm::X, glm::Z)
#define zzxz swizzle(glm::Z, glm::Z, glm::X, glm::Z)
#define wzxz swizzle(glm::W, glm::Z, glm::X, glm::Z)
#define xwxz swizzle(glm::X, glm::W, glm::X, glm::Z)
#define ywxz swizzle(glm::Y, glm::W, glm::X, glm::Z)
#define zwxz swizzle(glm::Z, glm::W, glm::X, glm::Z)
#define wwxz swizzle(glm::W, glm::W, glm::X, glm::Z)
#define xxyz swizzle(glm::X, glm::X, glm::Y, glm::Z)
#define yxyz swizzle(glm::Y, glm::X, glm::Y, glm::Z)
#define zxyz swizzle(glm::Z, glm::X, glm::Y, glm::Z)
#define wxyz swizzle(glm::W, glm::X, glm::Y, glm::Z)
#define xyyz swizzle(glm::X, glm::Y, glm::Y, glm::Z)
#define yyyz swizzle(glm::Y, glm::Y, glm::Y, glm::Z)
#define zyyz swizzle(glm::Z, glm::Y, glm::Y, glm::Z)
#define wyyz swizzle(glm::W, glm::Y, glm::Y, glm::Z)
#define xzyz swizzle(glm::X, glm::Z, glm::Y, glm::Z)
#define yzyz swizzle(glm::Y, glm::Z, glm::Y, glm::Z)
#define zzyz swizzle(glm::Z, glm::Z, glm::Y, glm::Z)
#define wzyz swizzle(glm::W, glm::Z, glm::Y, glm::Z)
#define xwyz swizzle(glm::X, glm::W, glm::Y, glm::Z)
#define ywyz swizzle(glm::Y, glm::W, glm::Y, glm::Z)
#define zwyz swizzle(glm::Z, glm::W, glm::Y, glm::Z)
#define wwyz swizzle(glm::W, glm::W, glm::Y, glm::Z)
#define xxzz swizzle(glm::X, glm::X, glm::Z, glm::Z)
#define yxzz swizzle(glm::Y, glm::X, glm::Z, glm::Z)
#define zxzz swizzle(glm::Z, glm::X, glm::Z, glm::Z)
#define wxzz swizzle(glm::W, glm::X, glm::Z, glm::Z)
#define xyzz swizzle(glm::X, glm::Y, glm::Z, glm::Z)
#define yyzz swizzle(glm::Y, glm::Y, glm::Z, glm::Z)
#define zyzz swizzle(glm::Z, glm::Y, glm::Z, glm::Z)
#define wyzz swizzle(glm::W, glm::Y, glm::Z, glm::Z)
#define xzzz swizzle(glm::X, glm::Z, glm::Z, glm::Z)
#define yzzz swizzle(glm::Y, glm::Z, glm::Z, glm::Z)
#define zzzz swizzle(glm::Z, glm::Z, glm::Z, glm::Z)
#define wzzz swizzle(glm::W, glm::Z, glm::Z, glm::Z)
#define xwzz swizzle(glm::X, glm::W, glm::Z, glm::Z)
#define ywzz swizzle(glm::Y, glm::W, glm::Z, glm::Z)
#define zwzz swizzle(glm::Z, glm::W, glm::Z, glm::Z)
#define wwzz swizzle(glm::W, glm::W, glm::Z, glm::Z)
#define xxwz swizzle(glm::X, glm::X, glm::W, glm::Z)
#define yxwz swizzle(glm::Y, glm::X, glm::W, glm::Z)
#define zxwz swizzle(glm::Z, glm::X, glm::W, glm::Z)
#define wxwz swizzle(glm::W, glm::X, glm::W, glm::Z)
#define xywz swizzle(glm::X, glm::Y, glm::W, glm::Z)
#define yywz swizzle(glm::Y, glm::Y, glm::W, glm::Z)
#define zywz swizzle(glm::Z, glm::Y, glm::W, glm::Z)
#define wywz swizzle(glm::W, glm::Y, glm::W, glm::Z)
#define xzwz swizzle(glm::X, glm::Z, glm::W, glm::Z)
#define yzwz swizzle(glm::Y, glm::Z, glm::W, glm::Z)
#define zzwz swizzle(glm::Z, glm::Z, glm::W, glm::Z)
#define wzwz swizzle(glm::W, glm::Z, glm::W, glm::Z)
#define xwwz swizzle(glm::X, glm::W, glm::W, glm::Z)
#define ywwz swizzle(glm::Y, glm::W, glm::W, glm::Z)
#define zwwz swizzle(glm::Z, glm::W, glm::W, glm::Z)
#define wwwz swizzle(glm::W, glm::W, glm::W, glm::Z)
#define xxxw swizzle(glm::X, glm::X, glm::X, glm::W)
#define yxxw swizzle(glm::Y, glm::X, glm::X, glm::W)
#define zxxw swizzle(glm::Z, glm::X, glm::X, glm::W)
#define wxxw swizzle(glm::W, glm::X, glm::X, glm::W)
#define xyxw swizzle(glm::X, glm::Y, glm::X, glm::W)
#define yyxw swizzle(glm::Y, glm::Y, glm::X, glm::W)
#define zyxw swizzle(glm::Z, glm::Y, glm::X, glm::W)
#define wyxw swizzle(glm::W, glm::Y, glm::X, glm::W)
#define xzxw swizzle(glm::X, glm::Z, glm::X, glm::W)
#define yzxw swizzle(glm::Y, glm::Z, glm::X, glm::W)
#define zzxw swizzle(glm::Z, glm::Z, glm::X, glm::W)
#define wzxw swizzle(glm::W, glm::Z, glm::X, glm::W)
#define xwxw swizzle(glm::X, glm::W, glm::X, glm::W)
#define ywxw swizzle(glm::Y, glm::W, glm::X, glm::W)
#define zwxw swizzle(glm::Z, glm::W, glm::X, glm::W)
#define wwxw swizzle(glm::W, glm::W, glm::X, glm::W)
#define xxyw swizzle(glm::X, glm::X, glm::Y, glm::W)
#define yxyw swizzle(glm::Y, glm::X, glm::Y, glm::W)
#define zxyw swizzle(glm::Z, glm::X, glm::Y, glm::W)
#define wxyw swizzle(glm::W, glm::X, glm::Y, glm::W)
#define xyyw swizzle(glm::X, glm::Y, glm::Y, glm::W)
#define yyyw swizzle(glm::Y, glm::Y, glm::Y, glm::W)
#define zyyw swizzle(glm::Z, glm::Y, glm::Y, glm::W)
#define wyyw swizzle(glm::W, glm::Y, glm::Y, glm::W)
#define xzyw swizzle(glm::X, glm::Z, glm::Y, glm::W)
#define yzyw swizzle(glm::Y, glm::Z, glm::Y, glm::W)
#define zzyw swizzle(glm::Z, glm::Z, glm::Y, glm::W)
#define wzyw swizzle(glm::W, glm::Z, glm::Y, glm::W)
#define xwyw swizzle(glm::X, glm::W, glm::Y, glm::W)
#define ywyw swizzle(glm::Y, glm::W, glm::Y, glm::W)
#define zwyw swizzle(glm::Z, glm::W, glm::Y, glm::W)
#define wwyw swizzle(glm::W, glm::W, glm::Y, glm::W)
#define xxzw swizzle(glm::X, glm::X, glm::Z, glm::W)
#define yxzw swizzle(glm::Y, glm::X, glm::Z, glm::W)
#define zxzw swizzle(glm::Z, glm::X, glm::Z, glm::W)
#define wxzw swizzle(glm::W, glm::X, glm::Z, glm::W)
#define xyzw swizzle(glm::X, glm::Y, glm::Z, glm::W)
#define yyzw swizzle(glm::Y, glm::Y, glm::Z, glm::W)
#define zyzw swizzle(glm::Z, glm::Y, glm::Z, glm::W)
#define wyzw swizzle(glm::W, glm::Y, glm::Z, glm::W)
#define xzzw swizzle(glm::X, glm::Z, glm::Z, glm::W)
#define yzzw swizzle(glm::Y, glm::Z, glm::Z, glm::W)
#define zzzw swizzle(glm::Z, glm::Z, glm::Z, glm::W)
#define wzzw swizzle(glm::W, glm::Z, glm::Z, glm::W)
#define xwzw swizzle(glm::X, glm::W, glm::Z, glm::W)
#define ywzw swizzle(glm::Y, glm::W, glm::Z, glm::W)
#define zwzw swizzle(glm::Z, glm::W, glm::Z, glm::W)
#define wwzw swizzle(glm::W, glm::W, glm::Z, glm::W)
#define xxww swizzle(glm::X, glm::X, glm::W, glm::W)
#define yxww swizzle(glm::Y, glm::X, glm::W, glm::W)
#define zxww swizzle(glm::Z, glm::X, glm::W, glm::W)
#define wxww swizzle(glm::W, glm::X, glm::W, glm::W)
#define xyww swizzle(glm::X, glm::Y, glm::W, glm::W)
#define yyww swizzle(glm::Y, glm::Y, glm::W, glm::W)
#define zyww swizzle(glm::Z, glm::Y, glm::W, glm::W)
#define wyww swizzle(glm::W, glm::Y, glm::W, glm::W)
#define xzww swizzle(glm::X, glm::Z, glm::W, glm::W)
#define yzww swizzle(glm::Y, glm::Z, glm::W, glm::W)
#define zzww swizzle(glm::Z, glm::Z, glm::W, glm::W)
#define wzww swizzle(glm::W, glm::Z, glm::W, glm::W)
#define xwww swizzle(glm::X, glm::W, glm::W, glm::W)
#define ywww swizzle(glm::Y, glm::W, glm::W, glm::W)
#define zwww swizzle(glm::Z, glm::W, glm::W, glm::W)
#define wwww swizzle(glm::W, glm::W, glm::W, glm::W)

#endif

#if(defined(GLM_SWIZZLE_RGBA) || defined(GLM_SWIZZLE))

#define rrrr swizzle(glm::X, glm::X, glm::X, glm::X)
#define grrr swizzle(glm::Y, glm::X, glm::X, glm::X)
#define brrr swizzle(glm::Z, glm::X, glm::X, glm::X)
#define arrr swizzle(glm::W, glm::X, glm::X, glm::X)
#define rgrr swizzle(glm::X, glm::Y, glm::X, glm::X)
#define ggrr swizzle(glm::Y, glm::Y, glm::X, glm::X)
#define bgrr swizzle(glm::Z, glm::Y, glm::X, glm::X)
#define agrr swizzle(glm::W, glm::Y, glm::X, glm::X)
#define rbrr swizzle(glm::X, glm::Z, glm::X, glm::X)
#define gbrr swizzle(glm::Y, glm::Z, glm::X, glm::X)
#define bbrr swizzle(glm::Z, glm::Z, glm::X, glm::X)
#define abrr swizzle(glm::W, glm::Z, glm::X, glm::X)
#define rarr swizzle(glm::X, glm::W, glm::X, glm::X)
#define garr swizzle(glm::Y, glm::W, glm::X, glm::X)
#define barr swizzle(glm::Z, glm::W, glm::X, glm::X)
#define aarr swizzle(glm::W, glm::W, glm::X, glm::X)
#define rrgr swizzle(glm::X, glm::X, glm::Y, glm::X)
#define grgr swizzle(glm::Y, glm::X, glm::Y, glm::X)
#define brgr swizzle(glm::Z, glm::X, glm::Y, glm::X)
#define argr swizzle(glm::W, glm::X, glm::Y, glm::X)
#define rggr swizzle(glm::X, glm::Y, glm::Y, glm::X)
#define gggr swizzle(glm::Y, glm::Y, glm::Y, glm::X)
#define bggr swizzle(glm::Z, glm::Y, glm::Y, glm::X)
#define aggr swizzle(glm::W, glm::Y, glm::Y, glm::X)
#define rbgr swizzle(glm::X, glm::Z, glm::Y, glm::X)
#define gbgr swizzle(glm::Y, glm::Z, glm::Y, glm::X)
#define bbgr swizzle(glm::Z, glm::Z, glm::Y, glm::X)
#define abgr swizzle(glm::W, glm::Z, glm::Y, glm::X)
#define ragr swizzle(glm::X, glm::W, glm::Y, glm::X)
#define gagr swizzle(glm::Y, glm::W, glm::Y, glm::X)
#define bagr swizzle(glm::Z, glm::W, glm::Y, glm::X)
#define aagr swizzle(glm::W, glm::W, glm::Y, glm::X)
#define rrbr swizzle(glm::X, glm::X, glm::Z, glm::X)
#define grbr swizzle(glm::Y, glm::X, glm::Z, glm::X)
#define brbr swizzle(glm::Z, glm::X, glm::Z, glm::X)
#define arbr swizzle(glm::W, glm::X, glm::Z, glm::X)
#define rgbr swizzle(glm::X, glm::Y, glm::Z, glm::X)
#define ggbr swizzle(glm::Y, glm::Y, glm::Z, glm::X)
#define bgbr swizzle(glm::Z, glm::Y, glm::Z, glm::X)
#define agbr swizzle(glm::W, glm::Y, glm::Z, glm::X)
#define rbbr swizzle(glm::X, glm::Z, glm::Z, glm::X)
#define gbbr swizzle(glm::Y, glm::Z, glm::Z, glm::X)
#define bbbr swizzle(glm::Z, glm::Z, glm::Z, glm::X)
#define abbr swizzle(glm::W, glm::Z, glm::Z, glm::X)
#define rabr swizzle(glm::X, glm::W, glm::Z, glm::X)
#define gabr swizzle(glm::Y, glm::W, glm::Z, glm::X)
#define babr swizzle(glm::Z, glm::W, glm::Z, glm::X)
#define aabr swizzle(glm::W, glm::W, glm::Z, glm::X)
#define rrar swizzle(glm::X, glm::X, glm::W, glm::X)
#define grar swizzle(glm::Y, glm::X, glm::W, glm::X)
#define brar swizzle(glm::Z, glm::X, glm::W, glm::X)
#define arar swizzle(glm::W, glm::X, glm::W, glm::X)
#define rgar swizzle(glm::X, glm::Y, glm::W, glm::X)
#define ggar swizzle(glm::Y, glm::Y, glm::W, glm::X)
#define bgar swizzle(glm::Z, glm::Y, glm::W, glm::X)
#define agar swizzle(glm::W, glm::Y, glm::W, glm::X)
#define rbar swizzle(glm::X, glm::Z, glm::W, glm::X)
#define gbar swizzle(glm::Y, glm::Z, glm::W, glm::X)
#define bbar swizzle(glm::Z, glm::Z, glm::W, glm::X)
#define abar swizzle(glm::W, glm::Z, glm::W, glm::X)
#define raar swizzle(glm::X, glm::W, glm::W, glm::X)
#define gaar swizzle(glm::Y, glm::W, glm::W, glm::X)
#define baar swizzle(glm::Z, glm::W, glm::W, glm::X)
#define aaar swizzle(glm::W, glm::W, glm::W, glm::X)
#define rrrg swizzle(glm::X, glm::X, glm::X, glm::Y)
#define grrg swizzle(glm::Y, glm::X, glm::X, glm::Y)
#define brrg swizzle(glm::Z, glm::X, glm::X, glm::Y)
#define arrg swizzle(glm::W, glm::X, glm::X, glm::Y)
#define rgrg swizzle(glm::X, glm::Y, glm::X, glm::Y)
#define ggrg swizzle(glm::Y, glm::Y, glm::X, glm::Y)
#define bgrg swizzle(glm::Z, glm::Y, glm::X, glm::Y)
#define agrg swizzle(glm::W, glm::Y, glm::X, glm::Y)
#define rbrg swizzle(glm::X, glm::Z, glm::X, glm::Y)
#define gbrg swizzle(glm::Y, glm::Z, glm::X, glm::Y)
#define bbrg swizzle(glm::Z, glm::Z, glm::X, glm::Y)
#define abrg swizzle(glm::W, glm::Z, glm::X, glm::Y)
#define rarg swizzle(glm::X, glm::W, glm::X, glm::Y)
#define garg swizzle(glm::Y, glm::W, glm::X, glm::Y)
#define barg swizzle(glm::Z, glm::W, glm::X, glm::Y)
#define aarg swizzle(glm::W, glm::W, glm::X, glm::Y)
#define rrgg swizzle(glm::X, glm::X, glm::Y, glm::Y)
#define grgg swizzle(glm::Y, glm::X, glm::Y, glm::Y)
#define brgg swizzle(glm::Z, glm::X, glm::Y, glm::Y)
#define argg swizzle(glm::W, glm::X, glm::Y, glm::Y)
#define rggg swizzle(glm::X, glm::Y, glm::Y, glm::Y)
#define gggg swizzle(glm::Y, glm::Y, glm::Y, glm::Y)
#define bggg swizzle(glm::Z, glm::Y, glm::Y, glm::Y)
#define aggg swizzle(glm::W, glm::Y, glm::Y, glm::Y)
#define rbgg swizzle(glm::X, glm::Z, glm::Y, glm::Y)
#define gbgg swizzle(glm::Y, glm::Z, glm::Y, glm::Y)
#define bbgg swizzle(glm::Z, glm::Z, glm::Y, glm::Y)
#define abgg swizzle(glm::W, glm::Z, glm::Y, glm::Y)
#define ragg swizzle(glm::X, glm::W, glm::Y, glm::Y)
#define gagg swizzle(glm::Y, glm::W, glm::Y, glm::Y)
#define bagg swizzle(glm::Z, glm::W, glm::Y, glm::Y)
#define aagg swizzle(glm::W, glm::W, glm::Y, glm::Y)
#define rrbg swizzle(glm::X, glm::X, glm::Z, glm::Y)
#define grbg swizzle(glm::Y, glm::X, glm::Z, glm::Y)
#define brbg swizzle(glm::Z, glm::X, glm::Z, glm::Y)
#define arbg swizzle(glm::W, glm::X, glm::Z, glm::Y)
#define rgbg swizzle(glm::X, glm::Y, glm::Z, glm::Y)
#define ggbg swizzle(glm::Y, glm::Y, glm::Z, glm::Y)
#define bgbg swizzle(glm::Z, glm::Y, glm::Z, glm::Y)
#define agbg swizzle(glm::W, glm::Y, glm::Z, glm::Y)
#define rbbg swizzle(glm::X, glm::Z, glm::Z, glm::Y)
#define gbbg swizzle(glm::Y, glm::Z, glm::Z, glm::Y)
#define bbbg swizzle(glm::Z, glm::Z, glm::Z, glm::Y)
#define abbg swizzle(glm::W, glm::Z, glm::Z, glm::Y)
#define rabg swizzle(glm::X, glm::W, glm::Z, glm::Y)
#define gabg swizzle(glm::Y, glm::W, glm::Z, glm::Y)
#define babg swizzle(glm::Z, glm::W, glm::Z, glm::Y)
#define aabg swizzle(glm::W, glm::W, glm::Z, glm::Y)
#define rrag swizzle(glm::X, glm::X, glm::W, glm::Y)
#define grag swizzle(glm::Y, glm::X, glm::W, glm::Y)
#define brag swizzle(glm::Z, glm::X, glm::W, glm::Y)
#define arag swizzle(glm::W, glm::X, glm::W, glm::Y)
#define rgag swizzle(glm::X, glm::Y, glm::W, glm::Y)
#define ggag swizzle(glm::Y, glm::Y, glm::W, glm::Y)
#define bgag swizzle(glm::Z, glm::Y, glm::W, glm::Y)
#define agag swizzle(glm::W, glm::Y, glm::W, glm::Y)
#define rbag swizzle(glm::X, glm::Z, glm::W, glm::Y)
#define gbag swizzle(glm::Y, glm::Z, glm::W, glm::Y)
#define bbag swizzle(glm::Z, glm::Z, glm::W, glm::Y)
#define abag swizzle(glm::W, glm::Z, glm::W, glm::Y)
#define raag swizzle(glm::X, glm::W, glm::W, glm::Y)
#define gaag swizzle(glm::Y, glm::W, glm::W, glm::Y)
#define baag swizzle(glm::Z, glm::W, glm::W, glm::Y)
#define aaag swizzle(glm::W, glm::W, glm::W, glm::Y)
#define rrrb swizzle(glm::X, glm::X, glm::X, glm::Z)
#define grrb swizzle(glm::Y, glm::X, glm::X, glm::Z)
#define brrb swizzle(glm::Z, glm::X, glm::X, glm::Z)
#define arrb swizzle(glm::W, glm::X, glm::X, glm::Z)
#define rgrb swizzle(glm::X, glm::Y, glm::X, glm::Z)
#define ggrb swizzle(glm::Y, glm::Y, glm::X, glm::Z)
#define bgrb swizzle(glm::Z, glm::Y, glm::X, glm::Z)
#define agrb swizzle(glm::W, glm::Y, glm::X, glm::Z)
#define rbrb swizzle(glm::X, glm::Z, glm::X, glm::Z)
#define gbrb swizzle(glm::Y, glm::Z, glm::X, glm::Z)
#define bbrb swizzle(glm::Z, glm::Z, glm::X, glm::Z)
#define abrb swizzle(glm::W, glm::Z, glm::X, glm::Z)
#define rarb swizzle(glm::X, glm::W, glm::X, glm::Z)
#define garb swizzle(glm::Y, glm::W, glm::X, glm::Z)
#define barb swizzle(glm::Z, glm::W, glm::X, glm::Z)
#define aarb swizzle(glm::W, glm::W, glm::X, glm::Z)
#define rrgb swizzle(glm::X, glm::X, glm::Y, glm::Z)
#define grgb swizzle(glm::Y, glm::X, glm::Y, glm::Z)
#define brgb swizzle(glm::Z, glm::X, glm::Y, glm::Z)
#define argb swizzle(glm::W, glm::X, glm::Y, glm::Z)
#define rggb swizzle(glm::X, glm::Y, glm::Y, glm::Z)
#define gggb swizzle(glm::Y, glm::Y, glm::Y, glm::Z)
#define bggb swizzle(glm::Z, glm::Y, glm::Y, glm::Z)
#define aggb swizzle(glm::W, glm::Y, glm::Y, glm::Z)
#define rbgb swizzle(glm::X, glm::Z, glm::Y, glm::Z)
#define gbgb swizzle(glm::Y, glm::Z, glm::Y, glm::Z)
#define bbgb swizzle(glm::Z, glm::Z, glm::Y, glm::Z)
#define abgb swizzle(glm::W, glm::Z, glm::Y, glm::Z)
#define ragb swizzle(glm::X, glm::W, glm::Y, glm::Z)
#define gagb swizzle(glm::Y, glm::W, glm::Y, glm::Z)
#define bagb swizzle(glm::Z, glm::W, glm::Y, glm::Z)
#define aagb swizzle(glm::W, glm::W, glm::Y, glm::Z)
#define rrbb swizzle(glm::X, glm::X, glm::Z, glm::Z)
#define grbb swizzle(glm::Y, glm::X, glm::Z, glm::Z)
#define brbb swizzle(glm::Z, glm::X, glm::Z, glm::Z)
#define arbb swizzle(glm::W, glm::X, glm::Z, glm::Z)
#define rgbb swizzle(glm::X, glm::Y, glm::Z, glm::Z)
#define ggbb swizzle(glm::Y, glm::Y, glm::Z, glm::Z)
#define bgbb swizzle(glm::Z, glm::Y, glm::Z, glm::Z)
#define agbb swizzle(glm::W, glm::Y, glm::Z, glm::Z)
#define rbbb swizzle(glm::X, glm::Z, glm::Z, glm::Z)
#define gbbb swizzle(glm::Y, glm::Z, glm::Z, glm::Z)
#define bbbb swizzle(glm::Z, glm::Z, glm::Z, glm::Z)
#define abbb swizzle(glm::W, glm::Z, glm::Z, glm::Z)
#define rabb swizzle(glm::X, glm::W, glm::Z, glm::Z)
#define gabb swizzle(glm::Y, glm::W, glm::Z, glm::Z)
#define babb swizzle(glm::Z, glm::W, glm::Z, glm::Z)
#define aabb swizzle(glm::W, glm::W, glm::Z, glm::Z)
#define rrab swizzle(glm::X, glm::X, glm::W, glm::Z)
#define grab swizzle(glm::Y, glm::X, glm::W, glm::Z)
#define brab swizzle(glm::Z, glm::X, glm::W, glm::Z)
#define arab swizzle(glm::W, glm::X, glm::W, glm::Z)
#define rgab swizzle(glm::X, glm::Y, glm::W, glm::Z)
#define ggab swizzle(glm::Y, glm::Y, glm::W, glm::Z)
#define bgab swizzle(glm::Z, glm::Y, glm::W, glm::Z)
#define agab swizzle(glm::W, glm::Y, glm::W, glm::Z)
#define rbab swizzle(glm::X, glm::Z, glm::W, glm::Z)
#define gbab swizzle(glm::Y, glm::Z, glm::W, glm::Z)
#define bbab swizzle(glm::Z, glm::Z, glm::W, glm::Z)
#define abab swizzle(glm::W, glm::Z, glm::W, glm::Z)
#define raab swizzle(glm::X, glm::W, glm::W, glm::Z)
#define gaab swizzle(glm::Y, glm::W, glm::W, glm::Z)
#define baab swizzle(glm::Z, glm::W, glm::W, glm::Z)
#define aaab swizzle(glm::W, glm::W, glm::W, glm::Z)
#define rrra swizzle(glm::X, glm::X, glm::X, glm::W)
#define grra swizzle(glm::Y, glm::X, glm::X, glm::W)
#define brra swizzle(glm::Z, glm::X, glm::X, glm::W)
#define arra swizzle(glm::W, glm::X, glm::X, glm::W)
#define rgra swizzle(glm::X, glm::Y, glm::X, glm::W)
#define ggra swizzle(glm::Y, glm::Y, glm::X, glm::W)
#define bgra swizzle(glm::Z, glm::Y, glm::X, glm::W)
#define agra swizzle(glm::W, glm::Y, glm::X, glm::W)
#define rbra swizzle(glm::X, glm::Z, glm::X, glm::W)
#define gbra swizzle(glm::Y, glm::Z, glm::X, glm::W)
#define bbra swizzle(glm::Z, glm::Z, glm::X, glm::W)
#define abra swizzle(glm::W, glm::Z, glm::X, glm::W)
#define rara swizzle(glm::X, glm::W, glm::X, glm::W)
#define gara swizzle(glm::Y, glm::W, glm::X, glm::W)
#define bara swizzle(glm::Z, glm::W, glm::X, glm::W)
#define aara swizzle(glm::W, glm::W, glm::X, glm::W)
#define rrga swizzle(glm::X, glm::X, glm::Y, glm::W)
#define grga swizzle(glm::Y, glm::X, glm::Y, glm::W)
#define brga swizzle(glm::Z, glm::X, glm::Y, glm::W)
#define arga swizzle(glm::W, glm::X, glm::Y, glm::W)
#define rgga swizzle(glm::X, glm::Y, glm::Y, glm::W)
#define ggga swizzle(glm::Y, glm::Y, glm::Y, glm::W)
#define bgga swizzle(glm::Z, glm::Y, glm::Y, glm::W)
#define agga swizzle(glm::W, glm::Y, glm::Y, glm::W)
#define rbga swizzle(glm::X, glm::Z, glm::Y, glm::W)
#define gbga swizzle(glm::Y, glm::Z, glm::Y, glm::W)
#define bbga swizzle(glm::Z, glm::Z, glm::Y, glm::W)
#define abga swizzle(glm::W, glm::Z, glm::Y, glm::W)
#define raga swizzle(glm::X, glm::W, glm::Y, glm::W)
#define gaga swizzle(glm::Y, glm::W, glm::Y, glm::W)
#define baga swizzle(glm::Z, glm::W, glm::Y, glm::W)
#define aaga swizzle(glm::W, glm::W, glm::Y, glm::W)
#define rrba swizzle(glm::X, glm::X, glm::Z, glm::W)
#define grba swizzle(glm::Y, glm::X, glm::Z, glm::W)
#define brba swizzle(glm::Z, glm::X, glm::Z, glm::W)
#define arba swizzle(glm::W, glm::X, glm::Z, glm::W)
#define rgba swizzle(glm::X, glm::Y, glm::Z, glm::W)
#define ggba swizzle(glm::Y, glm::Y, glm::Z, glm::W)
#define bgba swizzle(glm::Z, glm::Y, glm::Z, glm::W)
#define agba swizzle(glm::W, glm::Y, glm::Z, glm::W)
#define rbba swizzle(glm::X, glm::Z, glm::Z, glm::W)
#define gbba swizzle(glm::Y, glm::Z, glm::Z, glm::W)
#define bbba swizzle(glm::Z, glm::Z, glm::Z, glm::W)
#define abba swizzle(glm::W, glm::Z, glm::Z, glm::W)
#define raba swizzle(glm::X, glm::W, glm::Z, glm::W)
#define gaba swizzle(glm::Y, glm::W, glm::Z, glm::W)
#define baba swizzle(glm::Z, glm::W, glm::Z, glm::W)
#define aaba swizzle(glm::W, glm::W, glm::Z, glm::W)
#define rraa swizzle(glm::X, glm::X, glm::W, glm::W)
#define graa swizzle(glm::Y, glm::X, glm::W, glm::W)
#define braa swizzle(glm::Z, glm::X, glm::W, glm::W)
#define araa swizzle(glm::W, glm::X, glm::W, glm::W)
#define rgaa swizzle(glm::X, glm::Y, glm::W, glm::W)
#define ggaa swizzle(glm::Y, glm::Y, glm::W, glm::W)
#define bgaa swizzle(glm::Z, glm::Y, glm::W, glm::W)
#define agaa swizzle(glm::W, glm::Y, glm::W, glm::W)
#define rbaa swizzle(glm::X, glm::Z, glm::W, glm::W)
#define gbaa swizzle(glm::Y, glm::Z, glm::W, glm::W)
#define bbaa swizzle(glm::Z, glm::Z, glm::W, glm::W)
#define abaa swizzle(glm::W, glm::Z, glm::W, glm::W)
#define raaa swizzle(glm::X, glm::W, glm::W, glm::W)
#define gaaa swizzle(glm::Y, glm::W, glm::W, glm::W)
#define baaa swizzle(glm::Z, glm::W, glm::W, glm::W)
#define aaaa swizzle(glm::W, glm::W, glm::W, glm::W)

#endif

#if(defined(GLM_FORCE_SWIZZLE_STPQ) || defined(GLM_SWIZZLE))

#define ssss swizzle(glm::X, glm::X, glm::X, glm::X)
#define tsss swizzle(glm::Y, glm::X, glm::X, glm::X)
#define psss swizzle(glm::Z, glm::X, glm::X, glm::X)
#define qsss swizzle(glm::W, glm::X, glm::X, glm::X)
#define stss swizzle(glm::X, glm::Y, glm::X, glm::X)
#define ttss swizzle(glm::Y, glm::Y, glm::X, glm::X)
#define ptss swizzle(glm::Z, glm::Y, glm::X, glm::X)
#define qtss swizzle(glm::W, glm::Y, glm::X, glm::X)
#define spss swizzle(glm::X, glm::Z, glm::X, glm::X)
#define tpss swizzle(glm::Y, glm::Z, glm::X, glm::X)
#define ppss swizzle(glm::Z, glm::Z, glm::X, glm::X)
#define qpss swizzle(glm::W, glm::Z, glm::X, glm::X)
#define sqss swizzle(glm::X, glm::W, glm::X, glm::X)
#define tqss swizzle(glm::Y, glm::W, glm::X, glm::X)
#define pqss swizzle(glm::Z, glm::W, glm::X, glm::X)
#define qqss swizzle(glm::W, glm::W, glm::X, glm::X)
#define ssts swizzle(glm::X, glm::X, glm::Y, glm::X)
#define tsts swizzle(glm::Y, glm::X, glm::Y, glm::X)
#define psts swizzle(glm::Z, glm::X, glm::Y, glm::X)
#define qsts swizzle(glm::W, glm::X, glm::Y, glm::X)
#define stts swizzle(glm::X, glm::Y, glm::Y, glm::X)
#define ttts swizzle(glm::Y, glm::Y, glm::Y, glm::X)
#define ptts swizzle(glm::Z, glm::Y, glm::Y, glm::X)
#define qtts swizzle(glm::W, glm::Y, glm::Y, glm::X)
#define spts swizzle(glm::X, glm::Z, glm::Y, glm::X)
#define tpts swizzle(glm::Y, glm::Z, glm::Y, glm::X)
#define ppts swizzle(glm::Z, glm::Z, glm::Y, glm::X)
#define qpts swizzle(glm::W, glm::Z, glm::Y, glm::X)
#define sqts swizzle(glm::X, glm::W, glm::Y, glm::X)
#define tqts swizzle(glm::Y, glm::W, glm::Y, glm::X)
#define pqts swizzle(glm::Z, glm::W, glm::Y, glm::X)
#define qqts swizzle(glm::W, glm::W, glm::Y, glm::X)
#define ssps swizzle(glm::X, glm::X, glm::Z, glm::X)
#define tsps swizzle(glm::Y, glm::X, glm::Z, glm::X)
#define psps swizzle(glm::Z, glm::X, glm::Z, glm::X)
#define qsps swizzle(glm::W, glm::X, glm::Z, glm::X)
#define stps swizzle(glm::X, glm::Y, glm::Z, glm::X)
#define ttps swizzle(glm::Y, glm::Y, glm::Z, glm::X)
#define ptps swizzle(glm::Z, glm::Y, glm::Z, glm::X)
#define qtps swizzle(glm::W, glm::Y, glm::Z, glm::X)
#define spps swizzle(glm::X, glm::Z, glm::Z, glm::X)
#define tpps swizzle(glm::Y, glm::Z, glm::Z, glm::X)
#define ppps swizzle(glm::Z, glm::Z, glm::Z, glm::X)
#define qpps swizzle(glm::W, glm::Z, glm::Z, glm::X)
#define sqps swizzle(glm::X, glm::W, glm::Z, glm::X)
#define tqps swizzle(glm::Y, glm::W, glm::Z, glm::X)
#define pqps swizzle(glm::Z, glm::W, glm::Z, glm::X)
#define qqps swizzle(glm::W, glm::W, glm::Z, glm::X)
#define ssqs swizzle(glm::X, glm::X, glm::W, glm::X)
#define tsqs swizzle(glm::Y, glm::X, glm::W, glm::X)
#define psqs swizzle(glm::Z, glm::X, glm::W, glm::X)
#define qsqs swizzle(glm::W, glm::X, glm::W, glm::X)
#define stqs swizzle(glm::X, glm::Y, glm::W, glm::X)
#define ttqs swizzle(glm::Y, glm::Y, glm::W, glm::X)
#define ptqs swizzle(glm::Z, glm::Y, glm::W, glm::X)
#define qtqs swizzle(glm::W, glm::Y, glm::W, glm::X)
#define spqs swizzle(glm::X, glm::Z, glm::W, glm::X)
#define tpqs swizzle(glm::Y, glm::Z, glm::W, glm::X)
#define ppqs swizzle(glm::Z, glm::Z, glm::W, glm::X)
#define qpqs swizzle(glm::W, glm::Z, glm::W, glm::X)
#define sqqs swizzle(glm::X, glm::W, glm::W, glm::X)
#define tqqs swizzle(glm::Y, glm::W, glm::W, glm::X)
#define pqqs swizzle(glm::Z, glm::W, glm::W, glm::X)
#define qqqs swizzle(glm::W, glm::W, glm::W, glm::X)
#define ssst swizzle(glm::X, glm::X, glm::X, glm::Y)
#define tsst swizzle(glm::Y, glm::X, glm::X, glm::Y)
#define psst swizzle(glm::Z, glm::X, glm::X, glm::Y)
#define qsst swizzle(glm::W, glm::X, glm::X, glm::Y)
#define stst swizzle(glm::X, glm::Y, glm::X, glm::Y)
#define ttst swizzle(glm::Y, glm::Y, glm::X, glm::Y)
#define ptst swizzle(glm::Z, glm::Y, glm::X, glm::Y)
#define qtst swizzle(glm::W, glm::Y, glm::X, glm::Y)
#define spst swizzle(glm::X, glm::Z, glm::X, glm::Y)
#define tpst swizzle(glm::Y, glm::Z, glm::X, glm::Y)
#define ppst swizzle(glm::Z, glm::Z, glm::X, glm::Y)
#define qpst swizzle(glm::W, glm::Z, glm::X, glm::Y)
#define sqst swizzle(glm::X, glm::W, glm::X, glm::Y)
#define tqst swizzle(glm::Y, glm::W, glm::X, glm::Y)
#define pqst swizzle(glm::Z, glm::W, glm::X, glm::Y)
#define qqst swizzle(glm::W, glm::W, glm::X, glm::Y)
#define sstt swizzle(glm::X, glm::X, glm::Y, glm::Y)
#define tstt swizzle(glm::Y, glm::X, glm::Y, glm::Y)
#define pstt swizzle(glm::Z, glm::X, glm::Y, glm::Y)
#define qstt swizzle(glm::W, glm::X, glm::Y, glm::Y)
#define sttt swizzle(glm::X, glm::Y, glm::Y, glm::Y)
#define tttt swizzle(glm::Y, glm::Y, glm::Y, glm::Y)
#define pttt swizzle(glm::Z, glm::Y, glm::Y, glm::Y)
#define qttt swizzle(glm::W, glm::Y, glm::Y, glm::Y)
#define sptt swizzle(glm::X, glm::Z, glm::Y, glm::Y)
#define tptt swizzle(glm::Y, glm::Z, glm::Y, glm::Y)
#define pptt swizzle(glm::Z, glm::Z, glm::Y, glm::Y)
#define qptt swizzle(glm::W, glm::Z, glm::Y, glm::Y)
#define sqtt swizzle(glm::X, glm::W, glm::Y, glm::Y)
#define tqtt swizzle(glm::Y, glm::W, glm::Y, glm::Y)
#define pqtt swizzle(glm::Z, glm::W, glm::Y, glm::Y)
#define qqtt swizzle(glm::W, glm::W, glm::Y, glm::Y)
#define sspt swizzle(glm::X, glm::X, glm::Z, glm::Y)
#define tspt swizzle(glm::Y, glm::X, glm::Z, glm::Y)
#define pspt swizzle(glm::Z, glm::X, glm::Z, glm::Y)
#define qspt swizzle(glm::W, glm::X, glm::Z, glm::Y)
#define stpt swizzle(glm::X, glm::Y, glm::Z, glm::Y)
#define ttpt swizzle(glm::Y, glm::Y, glm::Z, glm::Y)
#define ptpt swizzle(glm::Z, glm::Y, glm::Z, glm::Y)
#define qtpt swizzle(glm::W, glm::Y, glm::Z, glm::Y)
#define sppt swizzle(glm::X, glm::Z, glm::Z, glm::Y)
#define tppt swizzle(glm::Y, glm::Z, glm::Z, glm::Y)
#define pppt swizzle(glm::Z, glm::Z, glm::Z, glm::Y)
#define qppt swizzle(glm::W, glm::Z, glm::Z, glm::Y)
#define sqpt swizzle(glm::X, glm::W, glm::Z, glm::Y)
#define tqpt swizzle(glm::Y, glm::W, glm::Z, glm::Y)
#define pqpt swizzle(glm::Z, glm::W, glm::Z, glm::Y)
#define qqpt swizzle(glm::W, glm::W, glm::Z, glm::Y)
#define ssqt swizzle(glm::X, glm::X, glm::W, glm::Y)
#define tsqt swizzle(glm::Y, glm::X, glm::W, glm::Y)
#define psqt swizzle(glm::Z, glm::X, glm::W, glm::Y)
#define qsqt swizzle(glm::W, glm::X, glm::W, glm::Y)
#define stqt swizzle(glm::X, glm::Y, glm::W, glm::Y)
#define ttqt swizzle(glm::Y, glm::Y, glm::W, glm::Y)
#define ptqt swizzle(glm::Z, glm::Y, glm::W, glm::Y)
#define qtqt swizzle(glm::W, glm::Y, glm::W, glm::Y)
#define spqt swizzle(glm::X, glm::Z, glm::W, glm::Y)
#define tpqt swizzle(glm::Y, glm::Z, glm::W, glm::Y)
#define ppqt swizzle(glm::Z, glm::Z, glm::W, glm::Y)
#define qpqt swizzle(glm::W, glm::Z, glm::W, glm::Y)
#define sqqt swizzle(glm::X, glm::W, glm::W, glm::Y)
#define tqqt swizzle(glm::Y, glm::W, glm::W, glm::Y)
#define pqqt swizzle(glm::Z, glm::W, glm::W, glm::Y)
#define qqqt swizzle(glm::W, glm::W, glm::W, glm::Y)
#define sssp swizzle(glm::X, glm::X, glm::X, glm::Z)
#define tssp swizzle(glm::Y, glm::X, glm::X, glm::Z)
#define pssp swizzle(glm::Z, glm::X, glm::X, glm::Z)
#define qssp swizzle(glm::W, glm::X, glm::X, glm::Z)
#define stsp swizzle(glm::X, glm::Y, glm::X, glm::Z)
#define ttsp swizzle(glm::Y, glm::Y, glm::X, glm::Z)
#define ptsp swizzle(glm::Z, glm::Y, glm::X, glm::Z)
#define qtsp swizzle(glm::W, glm::Y, glm::X, glm::Z)
#define spsp swizzle(glm::X, glm::Z, glm::X, glm::Z)
#define tpsp swizzle(glm::Y, glm::Z, glm::X, glm::Z)
#define ppsp swizzle(glm::Z, glm::Z, glm::X, glm::Z)
#define qpsp swizzle(glm::W, glm::Z, glm::X, glm::Z)
#define sqsp swizzle(glm::X, glm::W, glm::X, glm::Z)
#define tqsp swizzle(glm::Y, glm::W, glm::X, glm::Z)
#define pqsp swizzle(glm::Z, glm::W, glm::X, glm::Z)
#define qqsp swizzle(glm::W, glm::W, glm::X, glm::Z)
#define sstp swizzle(glm::X, glm::X, glm::Y, glm::Z)
#define tstp swizzle(glm::Y, glm::X, glm::Y, glm::Z)
#define pstp swizzle(glm::Z, glm::X, glm::Y, glm::Z)
#define qstp swizzle(glm::W, glm::X, glm::Y, glm::Z)
#define sttp swizzle(glm::X, glm::Y, glm::Y, glm::Z)
#define tttp swizzle(glm::Y, glm::Y, glm::Y, glm::Z)
#define pttp swizzle(glm::Z, glm::Y, glm::Y, glm::Z)
#define qttp swizzle(glm::W, glm::Y, glm::Y, glm::Z)
#define sptp swizzle(glm::X, glm::Z, glm::Y, glm::Z)
#define tptp swizzle(glm::Y, glm::Z, glm::Y, glm::Z)
#define pptp swizzle(glm::Z, glm::Z, glm::Y, glm::Z)
#define qptp swizzle(glm::W, glm::Z, glm::Y, glm::Z)
#define sqtp swizzle(glm::X, glm::W, glm::Y, glm::Z)
#define tqtp swizzle(glm::Y, glm::W, glm::Y, glm::Z)
#define pqtp swizzle(glm::Z, glm::W, glm::Y, glm::Z)
#define qqtp swizzle(glm::W, glm::W, glm::Y, glm::Z)
#define sspp swizzle(glm::X, glm::X, glm::Z, glm::Z)
#define tspp swizzle(glm::Y, glm::X, glm::Z, glm::Z)
#define pspp swizzle(glm::Z, glm::X, glm::Z, glm::Z)
#define qspp swizzle(glm::W, glm::X, glm::Z, glm::Z)
#define stpp swizzle(glm::X, glm::Y, glm::Z, glm::Z)
#define ttpp swizzle(glm::Y, glm::Y, glm::Z, glm::Z)
#define ptpp swizzle(glm::Z, glm::Y, glm::Z, glm::Z)
#define qtpp swizzle(glm::W, glm::Y, glm::Z, glm::Z)
#define sppp swizzle(glm::X, glm::Z, glm::Z, glm::Z)
#define tppp swizzle(glm::Y, glm::Z, glm::Z, glm::Z)
#define pppp swizzle(glm::Z, glm::Z, glm::Z, glm::Z)
#define qppp swizzle(glm::W, glm::Z, glm::Z, glm::Z)
#define sqpp swizzle(glm::X, glm::W, glm::Z, glm::Z)
#define tqpp swizzle(glm::Y, glm::W, glm::Z, glm::Z)
#define pqpp swizzle(glm::Z, glm::W, glm::Z, glm::Z)
#define qqpp swizzle(glm::W, glm::W, glm::Z, glm::Z)
#define ssqp swizzle(glm::X, glm::X, glm::W, glm::Z)
#define tsqp swizzle(glm::Y, glm::X, glm::W, glm::Z)
#define psqp swizzle(glm::Z, glm::X, glm::W, glm::Z)
#define qsqp swizzle(glm::W, glm::X, glm::W, glm::Z)
#define stqp swizzle(glm::X, glm::Y, glm::W, glm::Z)
#define ttqp swizzle(glm::Y, glm::Y, glm::W, glm::Z)
#define ptqp swizzle(glm::Z, glm::Y, glm::W, glm::Z)
#define qtqp swizzle(glm::W, glm::Y, glm::W, glm::Z)
#define spqp swizzle(glm::X, glm::Z, glm::W, glm::Z)
#define tpqp swizzle(glm::Y, glm::Z, glm::W, glm::Z)
#define ppqp swizzle(glm::Z, glm::Z, glm::W, glm::Z)
#define qpqp swizzle(glm::W, glm::Z, glm::W, glm::Z)
#define sqqp swizzle(glm::X, glm::W, glm::W, glm::Z)
#define tqqp swizzle(glm::Y, glm::W, glm::W, glm::Z)
#define pqqp swizzle(glm::Z, glm::W, glm::W, glm::Z)
#define qqqp swizzle(glm::W, glm::W, glm::W, glm::Z)
#define sssq swizzle(glm::X, glm::X, glm::X, glm::W)
#define tssq swizzle(glm::Y, glm::X, glm::X, glm::W)
#define pssq swizzle(glm::Z, glm::X, glm::X, glm::W)
#define qssq swizzle(glm::W, glm::X, glm::X, glm::W)
#define stsq swizzle(glm::X, glm::Y, glm::X, glm::W)
#define ttsq swizzle(glm::Y, glm::Y, glm::X, glm::W)
#define ptsq swizzle(glm::Z, glm::Y, glm::X, glm::W)
#define qtsq swizzle(glm::W, glm::Y, glm::X, glm::W)
#define spsq swizzle(glm::X, glm::Z, glm::X, glm::W)
#define tpsq swizzle(glm::Y, glm::Z, glm::X, glm::W)
#define ppsq swizzle(glm::Z, glm::Z, glm::X, glm::W)
#define qpsq swizzle(glm::W, glm::Z, glm::X, glm::W)
#define sqsq swizzle(glm::X, glm::W, glm::X, glm::W)
#define tqsq swizzle(glm::Y, glm::W, glm::X, glm::W)
#define pqsq swizzle(glm::Z, glm::W, glm::X, glm::W)
#define qqsq swizzle(glm::W, glm::W, glm::X, glm::W)
#define sstq swizzle(glm::X, glm::X, glm::Y, glm::W)
#define tstq swizzle(glm::Y, glm::X, glm::Y, glm::W)
#define pstq swizzle(glm::Z, glm::X, glm::Y, glm::W)
#define qstq swizzle(glm::W, glm::X, glm::Y, glm::W)
#define sttq swizzle(glm::X, glm::Y, glm::Y, glm::W)
#define tttq swizzle(glm::Y, glm::Y, glm::Y, glm::W)
#define pttq swizzle(glm::Z, glm::Y, glm::Y, glm::W)
#define qttq swizzle(glm::W, glm::Y, glm::Y, glm::W)
#define sptq swizzle(glm::X, glm::Z, glm::Y, glm::W)
#define tptq swizzle(glm::Y, glm::Z, glm::Y, glm::W)
#define pptq swizzle(glm::Z, glm::Z, glm::Y, glm::W)
#define qptq swizzle(glm::W, glm::Z, glm::Y, glm::W)
#define sqtq swizzle(glm::X, glm::W, glm::Y, glm::W)
#define tqtq swizzle(glm::Y, glm::W, glm::Y, glm::W)
#define pqtq swizzle(glm::Z, glm::W, glm::Y, glm::W)
#define qqtq swizzle(glm::W, glm::W, glm::Y, glm::W)
#define sspq swizzle(glm::X, glm::X, glm::Z, glm::W)
#define tspq swizzle(glm::Y, glm::X, glm::Z, glm::W)
#define pspq swizzle(glm::Z, glm::X, glm::Z, glm::W)
#define qspq swizzle(glm::W, glm::X, glm::Z, glm::W)
#define stpq swizzle(glm::X, glm::Y, glm::Z, glm::W)
#define ttpq swizzle(glm::Y, glm::Y, glm::Z, glm::W)
#define ptpq swizzle(glm::Z, glm::Y, glm::Z, glm::W)
#define qtpq swizzle(glm::W, glm::Y, glm::Z, glm::W)
#define sppq swizzle(glm::X, glm::Z, glm::Z, glm::W)
#define tppq swizzle(glm::Y, glm::Z, glm::Z, glm::W)
#define pppq swizzle(glm::Z, glm::Z, glm::Z, glm::W)
#define qppq swizzle(glm::W, glm::Z, glm::Z, glm::W)
#define sqpq swizzle(glm::X, glm::W, glm::Z, glm::W)
#define tqpq swizzle(glm::Y, glm::W, glm::Z, glm::W)
#define pqpq swizzle(glm::Z, glm::W, glm::Z, glm::W)
#define qqpq swizzle(glm::W, glm::W, glm::Z, glm::W)
#define ssqq swizzle(glm::X, glm::X, glm::W, glm::W)
#define tsqq swizzle(glm::Y, glm::X, glm::W, glm::W)
#define psqq swizzle(glm::Z, glm::X, glm::W, glm::W)
#define qsqq swizzle(glm::W, glm::X, glm::W, glm::W)
#define stqq swizzle(glm::X, glm::Y, glm::W, glm::W)
#define ttqq swizzle(glm::Y, glm::Y, glm::W, glm::W)
#define ptqq swizzle(glm::Z, glm::Y, glm::W, glm::W)
#define qtqq swizzle(glm::W, glm::Y, glm::W, glm::W)
#define spqq swizzle(glm::X, glm::Z, glm::W, glm::W)
#define tpqq swizzle(glm::Y, glm::Z, glm::W, glm::W)
#define ppqq swizzle(glm::Z, glm::Z, glm::W, glm::W)
#define qpqq swizzle(glm::W, glm::Z, glm::W, glm::W)
#define sqqq swizzle(glm::X, glm::W, glm::W, glm::W)
#define tqqq swizzle(glm::Y, glm::W, glm::W, glm::W)
#define pqqq swizzle(glm::Z, glm::W, glm::W, glm::W)
#define qqqq swizzle(glm::W, glm::W, glm::W, glm::W)

#endif

#endif//glm_core_swizzle
