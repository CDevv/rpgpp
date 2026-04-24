// MIT License
//
// Copyright (c) 2017 Scott Slack-Smith
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once
#include <utility>
#include <typeinfo>
#include <type_traits>
#ifdef _MSC_VER
  #if _HAS_CXX17 != 0
    #include <optional>
    #define ANY_PTR_HAS_LIB_OPTIONAL 
  #endif
#else
  #if __has_include(<optional>) // requires GCC 5 or greater
    #include <optional>
  #else
    #include <experimental/optional>
    namespace std {
      using std::experimental::optional;
    } // namespace std
  #endif
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 26461 26493 26496) // disable various GSL warnings emitted the code analyzer
#endif

namespace xxx {

  class bad_any_ptr_cast : public std::bad_cast
  {
  public:
    virtual const char* what() const noexcept override { return "bad any_ptr cast"; }
  };

  inline namespace v1 {

    /**
      The class any_ptr is type-safe container for pointers to any type 
      that, unlike std::any,  supports cv-qualifier promotion and dynamic up-cast.
      
      For example:

        struct Base {};
        struct Derived : public Base {};

        auto ptr = std::make_unique<Derived>();
        Derived * p = ptr.get();

        std::any a1{ p };
        std::any_cast<Derived>( a1 ); // cast succeeds
        std::any_cast<Base>( a1 ); // cast fails as there's no implicit up cast

        xxx::any_ptr a2{ p };
        xxx::any_ptr_cast<Base>( a2 ); // implicit up cast succeeds
    */
    class any_ptr
    {
    public:
      //-----------------------------------------------------
      // Canonical "Rule of 5" ctors, dtor and copy operators

      ~any_ptr() = default;

      any_ptr(any_ptr const & other) noexcept = default;

      any_ptr(any_ptr && other) noexcept = default;

      any_ptr& operator=(any_ptr const& other) noexcept = default;

      any_ptr& operator=(any_ptr && other) noexcept = default;

      //-----------------------------------------------------
      // Constructors

      // set to empty state
      any_ptr() noexcept = default;

      template<typename T>
      any_ptr(T* ptr) noexcept
        : my_type_info{ &typeid(HeldType<T>) }
        , my_ptr{ const_cast<HeldType<T>> (ptr) }
        , my_throw_func{ &any_ptr::throw_function<HeldBaseType<T>> }
      {
      }

      //-----------------------------------------------------
      // Modifiers

      // Reset to empty state 
      void reset() noexcept
      {
        // Use default ctor to reset to empty state
        ::new (this) any_ptr();
      }

      // Swaps two any objects
      void swap(any_ptr & other) noexcept
      {
        other = std::exchange(*this, std::move(other));
      }

      //-----------------------------------------------------
      // Observers

      // Returns true if instance is non-empty.
      bool has_value() const noexcept { return my_throw_func != nullptr; }

      // Returns the typeid(T*) of the contained pointer T* if instance is non-empty,
      // otherwise typeid(void).
      const std::type_info& type() const noexcept { return *my_type_info; }

    private:

      template<typename T>
      using HeldBaseType = T;

      template<typename T>
      using HeldType = std::add_pointer_t<T>;

      using Throw_func = void(void *);

      // The typeid(T*) of the held pointer, 
      // otherwise set to typeid(void) to indicate an empty state.
      const std::type_info *  my_type_info{ & typeid(void) };
      // The held pointer
      void*                   my_ptr{ nullptr };
      // The throw function that implements a dynamic up cast
      Throw_func *            my_throw_func{ nullptr };

      // Attempt a dynamic up cast to T to replicate an implicit up cast. 
      // If the cast is successful then return { ptr , true } where ptr is the casted pointer
      // otherwise return { nullptr , false }.
      template <typename T>
      std::pair<T*,bool> dynamic_up_cast() const noexcept
      {
        std::pair<T*, bool> result{ nullptr, false };
        if (type() == typeid(HeldType<T>)) { // cast succeeded
          result.first = static_cast<T*>(my_ptr);
          result.second = true;
        }
        else if (has_value()) { // attempt a dynamic up cast by throwing an exception
          try {
            my_throw_func(const_cast<void*>(my_ptr));
          }
          catch (T* const ptr) { // up cast succeeded
            result.first = ptr;
            result.second = true;
          }
          catch (...) { // up cast failed
          }
        }
        // else dynamic up cast failed
        return result;
      }

      template<typename T>
      static void throw_function(void * const ptr)
      {
        throw static_cast<T*>(ptr);
      }

#ifdef ANY_PTR_HAS_LIB_OPTIONAL

      template<typename T>
      friend std::optional<T*> any_ptr_cast(any_ptr const * any_ptr_) noexcept;

#else // replace std::optional<T*> with std::pair<T*, bool>

      template<typename T>
      std::pair<T*, bool> any_ptr_cast(const any_ptr * any_ptr_) noexcept;

#endif

      template<typename T>
      friend T* any_ptr_cast(any_ptr const & any_ptr_);
    };

    //-----------------------------------------------------------------------------------------------------


#ifdef ANY_PTR_HAS_LIB_OPTIONAL

    template<typename T>
    std::optional<T*> any_ptr_cast(const any_ptr * any_ptr_) noexcept
    {
      std::optional<T*> result;
      const std::pair<T*, bool> cast_result = any_ptr_->template dynamic_up_cast<T>();
      if (cast_result.second)  {
        result = cast_result.first;
      }
      return result;
    }

#else // replace std::optional<T*> with std::pair<T*, bool>

    template<typename T>
    std::pair<T*, bool> any_ptr_cast(const any_ptr * any_ptr_) noexcept
    {
      return any_ptr_->template dynamic_up_cast<T>();
    }

#endif

    template<typename T>
    T* any_ptr_cast(any_ptr const & any_ptr_)
    {
      const std::pair<T*,bool> result = any_ptr_.template dynamic_up_cast<T>();
      if (result.second) {
        return result.first;
      }
      throw bad_any_ptr_cast();
    }

  } // namespace v2

} // namespace xxx


namespace std {

  inline void swap(xxx::any_ptr & lhs, xxx::any_ptr & rhs)
  {
    lhs.swap(rhs);
  }

} // namespace std

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifdef ANY_PTR_HAS_LIB_OPTIONAL 
#undef ANY_PTR_HAS_LIB_OPTIONAL 
#endif
