//
// Created by Andrei on 18-Apr-24.
//

#ifndef ANDREI_UTILS_POINTER_HPP
#define ANDREI_UTILS_POINTER_HPP

#include <AndreiUtils/traits/InstanceOf.hpp>
#include <concepts>
#include <memory>

namespace AndreiUtils {
    // the first condition is to handle non-class types such as int, float, double
    // (which fail the std::is_base_of<int, int> condition...)
    template<typename T, typename SubT>
    concept TypeWithSubTypes = std::is_same_v<std::remove_const_t<T>, std::remove_const_t<SubT>> ||
                               std::is_base_of_v<std::remove_const_t<T>, std::remove_const_t<SubT>>;

    template<typename T, typename SubT>
    concept StrictSubTypeOfT = std::is_base_of_v<std::remove_const_t<T>, std::remove_const_t<SubT>> &&
                               std::negation<std::is_same<std::remove_const_t<T>, std::remove_const_t<SubT>>>::value;

    template<typename T>
    concept NotAbstract = std::negation_v<std::is_abstract<T>>;

    template<typename T>
    concept NotConst = std::negation_v<std::is_const<T>>;

    template<typename T1, typename T2>
    concept TypesWithSameConst = (std::is_const_v<T1> == std::is_const_v<T2>);

    template<typename T1, typename T2>
    concept FirstConstSecondNot = (std::is_const_v<T1> && !std::is_const_v<T2>);

    template<typename T>
    class Pointer {
    public:
        using ElementType = T;
        using SmartPtrType = std::shared_ptr<ElementType>;

        Pointer();

        // no marking as explicit because we want the conversion from pointer to Pointer
        Pointer(std::nullptr_t);  // NOLINT(*-explicit-constructor)

        template<typename Type>
        requires TypeWithSubTypes<T, Type>
        explicit Pointer(Type &datum);

        template<typename Type>
        requires TypeWithSubTypes<T, Type>
        explicit Pointer(Type const &datum);

        template<typename Type>
        requires TypeWithSubTypes<T, Type>
        explicit Pointer(Type &&datum);

        // no marking as explicit because we want the conversion from pointer to Pointer
        Pointer(T *datum);  // NOLINT(google-explicit-constructor)

        // no marking as explicit because we want the conversion from pointer to Pointer
        Pointer(std::remove_const_t<T> *datum) requires std::is_const_v<T>;  // NOLINT(google-explicit-constructor)

        // no marking as explicit because we want the conversion from SmartPtrType to Pointer
        Pointer(SmartPtrType datum);  // NOLINT(google-explicit-constructor)

        // no marking as explicit because we want the conversion from SmartPtrType to Pointer
        template<typename SubT> requires StrictSubTypeOfT<T, SubT>
        Pointer(std::shared_ptr<SubT> datum);  // NOLINT(google-explicit-constructor)

        Pointer(Pointer const &other);   // NOLINT(*-explicit-constructor)

        Pointer(Pointer &&other) noexcept;   // NOLINT(*-explicit-constructor)

        Pointer(Pointer<std::remove_const_t<T>> const &other) requires std::is_const_v<T>;   // NOLINT(*-explicit-constructor)

        Pointer(Pointer<std::remove_const_t<T>> &&other) noexcept requires std::is_const_v<T>;   // NOLINT(*-explicit-constructor)

        // differentiation between StrictSubType is needed because of the protected (isRegular, ptr, smart)
        // members of Pointer<Type> which must be accessible from the constructor or Pointer<T>
        // (but they would not be accessible without this differentiation because of their protected status)

        template<typename Type>
        requires StrictSubTypeOfT<T, Type> && TypesWithSameConst<T, Type>
        Pointer(Pointer<Type> const &other);   // NOLINT(*-explicit-constructor)

        template<typename Type>
        requires StrictSubTypeOfT<T, Type> && TypesWithSameConst<T, Type>
        Pointer(Pointer<Type> &&other) noexcept;   // NOLINT(*-explicit-constructor)

        template<typename Type>
        requires StrictSubTypeOfT<T, Type> && FirstConstSecondNot<T, Type>
        Pointer(Pointer<Type> const &other);  // NOLINT(*-explicit-constructor)

        template<typename Type>
        requires StrictSubTypeOfT<T, Type> && FirstConstSecondNot<T, Type>
        Pointer(Pointer<Type> &&other) noexcept;  // NOLINT(*-explicit-constructor)

        template<typename Type>
        requires TypeWithSubTypes<T, Type>
        Pointer &operator=(Type &other);

        template<typename Type>
        requires TypeWithSubTypes<T, Type>
        Pointer &operator=(Type const &other);

        template<typename Type>
        requires TypeWithSubTypes<T, Type>
        Pointer &operator=(Type &&other);

        Pointer &operator=(std::nullptr_t);

        Pointer &operator=(T *other);

        Pointer &operator=(std::remove_const_t<T> *other) requires std::is_const_v<T>;

        Pointer &operator=(SmartPtrType other);

        template<typename SubT> requires StrictSubTypeOfT<T, SubT>
        Pointer &operator=(std::shared_ptr<SubT> other);

        Pointer &operator=(Pointer const &other);

        Pointer &operator=(Pointer &&other) noexcept;

        Pointer &operator=(Pointer<std::remove_const_t<T>> const &other) requires std::is_const_v<T>;

        Pointer &operator=(Pointer<std::remove_const_t<T>> &&other) noexcept requires std::is_const_v<T>;

        template<typename Type>
        requires StrictSubTypeOfT<T, Type> && TypesWithSameConst<T, Type>
        Pointer &operator=(Pointer<Type> const &other);

        template<typename Type>
        requires StrictSubTypeOfT<T, Type> && TypesWithSameConst<T, Type>
        Pointer &operator=(Pointer<Type> &&other) noexcept;

        template<typename Type>
        requires StrictSubTypeOfT<T, Type> && FirstConstSecondNot<T, Type>
        Pointer &operator=(Pointer<Type> const &other);

        template<typename Type>
        requires StrictSubTypeOfT<T, Type> && FirstConstSecondNot<T, Type>
        Pointer &operator=(Pointer<Type> &&other) noexcept;

        [[nodiscard]] bool operator<(Pointer<std::remove_const_t<T>> const &other) const;

        [[nodiscard]] bool operator<(Pointer<std::remove_const_t<T> const> const &other) const;

        explicit operator bool() const;

        [[nodiscard]] T *get() const & noexcept;

        [[nodiscard]] T *get() const && noexcept = delete;

        T *operator->() const & noexcept;

        T *operator->() const && noexcept = delete;

        T &operator*() const & noexcept requires NotAbstract<T>;

        T &operator*() const && noexcept requires NotAbstract<T> = delete;

        [[nodiscard]] T *ptrOfTempValue() const && noexcept;

        [[nodiscard]] T &&moveTempValue() const && noexcept;

        void reset();

        Pointer<T const> constCast() const noexcept requires NotConst<T>;

        Pointer<T const> constCastMove() && noexcept requires NotConst<T>;

        Pointer<std::remove_const_t<T>> constCast() const noexcept requires std::is_const_v<T>;

        Pointer<std::remove_const_t<T>> constCastMove() && noexcept requires std::is_const_v<T>;

        template<typename ParentCastT>
        requires TypeWithSubTypes<ParentCastT, T> && TypesWithSameConst<ParentCastT, T>
        Pointer<ParentCastT> cast() const noexcept;

        template<typename ParentCastT>
        requires TypeWithSubTypes<ParentCastT, T> && TypesWithSameConst<ParentCastT, T>
        Pointer<ParentCastT> castMove() && noexcept;

        template<typename CastT>
        Pointer<CastT> dynamicCast() const noexcept;

        template<typename CastT>
        Pointer<CastT> staticCast() const noexcept;

        template<typename CastT>
        Pointer<CastT> reinterpretCast() const noexcept;

        template<typename TypeCheck>
        [[nodiscard]] bool isInstanceOf() const;

        template<typename TypeCheck>
        [[nodiscard]] bool isInstanceOf(TypeCheck const *&res) const;

        [[nodiscard]] bool getIfPtrIsSmartPtr(SmartPtrType &smartRef) const;

    protected:
        bool isRegular;
        T *regular;
        SmartPtrType smart;
    };

    template<typename T1, typename T2>
    inline bool operator==(AndreiUtils::Pointer<T1> const &ptr1, AndreiUtils::Pointer<T2> const &ptr2) noexcept {
        return ptr1.get() == ptr2.get();
    }

    template<typename T>
    inline bool operator==(AndreiUtils::Pointer<T> const &ptr, std::nullptr_t) noexcept { return !ptr; }

    template<typename T>
    inline bool operator==(std::nullptr_t, AndreiUtils::Pointer<T> const &ptr) noexcept { return !ptr; }

    template<typename T1, typename T2>
    inline bool operator!=(AndreiUtils::Pointer<T1> const &ptr1, AndreiUtils::Pointer<T2> const &ptr2) noexcept {
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

    template<typename TypeCheck, typename InstanceType>
    bool pointerInstanceOf(Pointer<InstanceType> const &val, TypeCheck const *&res) {
        return val.template isInstanceOf<TypeCheck>(res);
    }
}

#include <AndreiUtils/classes/Pointer.tpp>

#endif  // ANDREI_UTILS_POINTER_HPP
