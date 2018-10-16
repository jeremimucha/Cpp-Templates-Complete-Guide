#pragma once

#include <type_traits>
#include <utility>
#include "try_equals.hpp"


// FunctorBridge
// The FunctorBridge is responsible for ownership and manipulation of the underlying function obj.
// It provides essential operations needed to manipulate stored function object through virtual
// interface: a destructor, a clone() and an invoke().
template<typename R, typename... Args>
class FunctorBridge
{
public:
    virtual ~FunctorBridge() noexcept = default;
    constexpr FunctorBridge() noexcept = default;
    FunctorBridge(FunctorBridge const&) = delete;
    FunctorBridge(FunctorBridge&&) = delete;
    FunctorBridge& operator=(FunctorBridge const&) = delete;
    FunctorBridge& operator=(FunctorBridge&&) = delete;

    virtual FunctorBridge* clone() const& = 0;
    virtual FunctorBridge* clone() && = 0;
    virtual R invoke(Args... args) const = 0;
    virtual bool equals(FunctorBridge const* fb) const = 0;
};

// Since instances of FunctorBridge are abstract classes, derived classes are responsible for
// providing actual implementations of its virtual functions.
// Since potential range of function objects is unbounded we would need an unbounded number
// of derived classes. We can achieve this by parameterizing the derived class on the type
// of the function objects it stores:
template<typename Functor, typename R, typename... Args>
class SpecificFunctorBridge final : public FunctorBridge<R, Args...>
{
private:
    Functor functor_;
public:
    template<typename FunctorFwd>
    constexpr SpecificFunctorBridge(FunctorFwd&& functor) noexcept
        : functor_{std::forward<FunctorFwd>(functor)} { }

    SpecificFunctorBridge* clone() const& override
    {
        return new SpecificFunctorBridge{functor_};
    }

    SpecificFunctorBridge* clone() && override
    {
        return new SpecificFunctorBridge{std::move(functor_)};
    }

    R invoke(Args... args) const override
    {
        return functor_(std::forward<Args>(args)...);
    }

    bool equals(FunctorBridge<R, Args...> const* fb) const override
    {
        if (auto spec_fb = dynamic_cast<SpecificFunctorBridge const*>(fb)) {
            // return functor_ == spec_fb->functor_;
        // only types which have operator==() can be compared
            return try_equals<Functor>::equals(functor_, spec_fb->functor_);
        }
        // functors with different types are never equal
        return false;
    }
};


// primary template
template<typename Signature>
class FunctionPtr;

// partial specialization
template<typename R, typename... Args>
class FunctionPtr<R(Args...)>   // Signature is R(Args...)
{
private:
    FunctorBridge<R, Args...>* bridge_{nullptr};
public:
    constexpr FunctionPtr() noexcept = default;

    FunctionPtr(FunctionPtr const& other)
        : bridge_{nullptr}
    {
        if (other.bridge_) {
            bridge_ = other.bridge_->clone();
        }
    }

    FunctionPtr(FunctionPtr& other)
        : FunctionPtr{static_cast<FunctionPtr const&>(other)} { }

    constexpr FunctionPtr(FunctionPtr&& other) noexcept
        : bridge_{std::exchange(other.bridge_, nullptr)} { }

    // construction from arbitrary function objects
    template<typename F>
    FunctionPtr(F&& f)
        : bridge_{new SpecificFunctorBridge<std::decay_t<F>, R, Args...>{std::forward<F>(f)}}
        { }

    // assignment
    FunctionPtr& operator=(FunctionPtr const& other)
    {
        auto tmp{other};
        using std::swap;
        swap(*this, tmp);
        return *this;
    }

    FunctionPtr& operator=(FunctionPtr&& other) noexcept
    {
        delete bridge_;
        bridge_ = std::exchange(other.bridge_, nullptr);
        return *this;
    }

    // construction and assignment from arbitrary function objects:
    template<typename F>
    constexpr FunctionPtr& operator=(F&& f) noexcept
    {
        FunctionPtr tmp{std::forward<F>(f)};
        using std::swap;
        swap(*this, tmp);
        return *this;
    }

    // destructor
    ~FunctionPtr() noexcept { delete bridge_; }

    friend void swap(FunctionPtr& lhs, FunctionPtr& rhs) noexcept
    {
        using std::swap;
        swap(lhs.bridge_, rhs.bridge_);
    }

    constexpr explicit operator bool() const noexcept { return bridge_ != nullptr; }

    // invocation
    R operator()(Args... args) const
    {
        return bridge_->invoke(std::forward<Args>(args)...);
    }

    friend bool operator==(FunctionPtr const& lhs, FunctionPtr const& rhs)
    {
        if (!lhs || !rhs) {
            return !lhs && !rhs;
        }
        return lhs.bridge_->equals(rhs.bridge_);
    }

    friend bool operator!=(FunctionPtr const& lhs, FunctionPtr const& rhs)
    {
        return !(lhs == rhs);
    }
};
