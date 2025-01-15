//
// Created by Andrei on 18-Apr-24.
//

#ifndef ANDREI_UTILS_POINTER_HPP
#define ANDREI_UTILS_POINTER_HPP

#include <AndreiUtils/traits/InstanceOf.hpp>
#include <concepts>
#include <memory>

namespace AndreiUtils {
    template<typename T, typename SubT>
    concept StrictSubTypeOfT = std::is_base_of<T, SubT>::value && std::negation<std::is_same<T, SubT>>::value;

    template<typename T>
    class Pointer {
    public:
        using ElementType = T;
        using SmartPtrType = std::shared_ptr<ElementType>;

        Pointer();

        // no marking as explicit because we want the conversion from pointer to Pointer
        Pointer(std::nullptr_t);  // NOLINT(*-explicit-constructor)

        template<typename SubT> requires StrictSubTypeOfT<T, SubT>
        explicit Pointer(SubT datum);

        explicit Pointer(T &datum);

        explicit Pointer(T const &datum);

        explicit Pointer(T &&datum);

        // no marking as explicit because we want the conversion from pointer to Pointer
        Pointer(T *datum);  // NOLINT(google-explicit-constructor)

        // no marking as explicit because we want the conversion from SmartPtrType to Pointer
        Pointer(SmartPtrType datum);  // NOLINT(google-explicit-constructor)

        Pointer(Pointer const &other);

        Pointer(Pointer &&other) noexcept;

        template<typename SubT> requires StrictSubTypeOfT<T, SubT>
        Pointer(Pointer<SubT> const &other);   // NOLINT(*-explicit-constructor)

        template<typename SubT> requires StrictSubTypeOfT<T, SubT>
        Pointer(Pointer<SubT> &&other) noexcept;  // NOLINT(*-explicit-constructor)

        template<typename SubT> requires StrictSubTypeOfT<T, SubT>
        Pointer &operator=(SubT other);

        Pointer &operator=(T &other);

        Pointer &operator=(T const &other);

        Pointer &operator=(T &&other);

        Pointer &operator=(std::nullptr_t);

        Pointer &operator=(T *other);

        Pointer &operator=(SmartPtrType other);

        Pointer &operator=(Pointer const &other);

        Pointer &operator=(Pointer &&other) noexcept;

        [[nodiscard]] bool operator<(Pointer const &other) const;

        [[nodiscard]] bool operator<(Pointer<T const> const &other) const;

        explicit operator bool() const;

        [[nodiscard]] T *get() const;

        T &operator*() const noexcept;

        T *operator->() const noexcept;

        void reset();

        Pointer<T const> constCast() const noexcept;

        Pointer<T const> constCastMove() && noexcept;

        template<typename ParentCastT> requires std::is_base_of<ParentCastT, T>::value
        Pointer<ParentCastT> cast() const noexcept;

        template<typename ParentCastT> requires std::is_base_of<ParentCastT, T>::value
        Pointer<ParentCastT> castMove() && noexcept;

        template<typename CastT>
        Pointer<CastT> dynamicCast() const noexcept;

        template<typename CastT>
        Pointer<CastT> staticCast() const noexcept;

        template<typename CastT>
        Pointer<CastT> reinterpretCast() const noexcept;

        template<typename TypeCheck>
        [[nodiscard]] bool isInstanceOf() const;

    protected:
        bool isRegular;
        T *ptr;
        SmartPtrType smart;
    };

    template<typename T>
    class Pointer<T const> {
    public:
        using ElementType = T const;
        using SmartPtrType = std::shared_ptr<ElementType>;

        Pointer();

        // no marking as explicit because we want the conversion from pointer to Pointer
        Pointer(std::nullptr_t);  // NOLINT(*-explicit-constructor)

        template<typename SubT> requires StrictSubTypeOfT<T, SubT>
        explicit Pointer(SubT const &datum);

        explicit Pointer(T &datum);

        explicit Pointer(T const &datum);

        explicit Pointer(T &&datum);

        // no marking as explicit because we want the conversion from pointer to Pointer
        Pointer(T *datum);  // NOLINT(*-explicit-constructor)

        // no marking as explicit because we want the conversion from pointer to Pointer
        Pointer(T const *datum);  // NOLINT(*-explicit-constructor)

        // no marking as explicit because we want the conversion from SmartPtrType to Pointer
        Pointer(SmartPtrType datum);  // NOLINT(google-explicit-constructor)

        Pointer(Pointer const &other);

        Pointer(Pointer &&other) noexcept;

        Pointer(Pointer<T> const &other);   // NOLINT(*-explicit-constructor)

        Pointer(Pointer<T> &&other) noexcept;  // NOLINT(*-explicit-constructor)

        template<typename SubT> requires StrictSubTypeOfT<T, SubT>
        Pointer(Pointer<SubT const> const &other);   // NOLINT(*-explicit-constructor)

        template<typename SubT> requires StrictSubTypeOfT<T, SubT>
        Pointer(Pointer<SubT const> &&other) noexcept;  // NOLINT(*-explicit-constructor)

        template<typename SubT> requires StrictSubTypeOfT<T, SubT>
        Pointer(Pointer<SubT> const &other);   // NOLINT(*-explicit-constructor)

        template<typename SubT> requires StrictSubTypeOfT<T, SubT>
        Pointer(Pointer<SubT> &&other) noexcept;  // NOLINT(*-explicit-constructor)

        template<typename SubT> requires StrictSubTypeOfT<T, SubT>
        Pointer &operator=(SubT const &other);

        Pointer &operator=(T &other);

        Pointer &operator=(T const &other);

        Pointer &operator=(T &&other);

        Pointer &operator=(std::nullptr_t);

        Pointer &operator=(T *other);

        Pointer &operator=(T const *other);

        Pointer &operator=(SmartPtrType other);

        Pointer &operator=(Pointer const &other);

        Pointer &operator=(Pointer &&other) noexcept;

        Pointer &operator=(Pointer<T> const &other);

        Pointer &operator=(Pointer<T> &&other) noexcept;

        [[nodiscard]] bool operator<(Pointer<T> const &other) const;

        [[nodiscard]] bool operator<(Pointer<T const> const &other) const;

        explicit operator bool() const;

        [[nodiscard]] T const *get() const;

        T const &operator*() const noexcept;

        T const *operator->() const noexcept;

        void reset();

        template<typename ParentCastT> requires std::is_base_of<ParentCastT, T>::value
        Pointer<ParentCastT const> cast() const noexcept;

        template<typename ParentCastT> requires std::is_base_of<ParentCastT, T>::value
        Pointer<ParentCastT const> castMove() && noexcept;

        template<typename CastT>
        Pointer<CastT> constCast() const noexcept;

        template<typename CastT>
        Pointer<CastT> dynamicCast() const noexcept;

        template<typename CastT>
        Pointer<CastT> staticCast() const noexcept;

        template<typename CastT>
        Pointer<CastT> reinterpretCast() const noexcept;

        template<typename TypeCheck>
        [[nodiscard]] bool isInstanceOf() const;

    protected:
        bool isRegular;
        T const *ptr;
        SmartPtrType smart;
    };

    template<typename T1, typename T2>
    inline bool
    operator==(AndreiUtils::Pointer<T1> const &ptr1, AndreiUtils::Pointer<T2> const &ptr2) noexcept {
        return ptr1.get() == ptr2.get();
    }

    template<typename T>
    inline bool operator==(AndreiUtils::Pointer<T> const &ptr, std::nullptr_t) noexcept { return !ptr; }

    template<typename T>
    inline bool operator==(std::nullptr_t, AndreiUtils::Pointer<T> const &ptr) noexcept { return !ptr; }

    template<typename T1, typename T2>
    inline bool
    operator!=(AndreiUtils::Pointer<T1> const &ptr1, AndreiUtils::Pointer<T2> const &ptr2) noexcept {
        return ptr1.get() != ptr2.get();
    }

    template<typename T>
    inline bool operator!=(AndreiUtils::Pointer<T> const &ptr, std::nullptr_t) noexcept { return (bool) ptr; }

    template<typename T>
    inline bool operator!=(std::nullptr_t, AndreiUtils::Pointer<T> const &ptr) noexcept { return (bool) ptr; }

    template<typename CastT, typename T>
    Pointer<CastT> static_pointer_cast(Pointer<T> const &p) {
        return p.template staticCast<CastT>();
    }

    template<typename CastT, typename T>
    Pointer<CastT> const_pointer_cast(Pointer<T> const &p) {
        return p.template constCast<CastT>();
    }

    template<typename CastT, typename T>
    Pointer<CastT> dynamic_pointer_cast(Pointer<T> const &p) {
        return p.template dynamicCast<CastT>();
    }

    template<typename CastT, typename T>
    Pointer<CastT> reinterpret_pointer_cast(Pointer<T> const &p) noexcept {
        return p.template reinterpretCast<CastT>();
    }

    template<typename TypeCheck, typename InstanceType>
    bool pointerInstanceOf(Pointer<InstanceType> const &val) {
        return val.template isInstanceOf<TypeCheck>();
    }
}

#include <AndreiUtils/classes/Pointer.tpp>
#include <AndreiUtils/classes/PointerConst.tpp>

#endif  // ANDREI_UTILS_POINTER_HPP
