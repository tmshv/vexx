#if !defined(CODE_GOOGLE_COM_P_V8_CONVERT_CLASS_CREATOR_HPP_INCLUDED)
#define CODE_GOOGLE_COM_P_V8_CONVERT_CLASS_CREATOR_HPP_INCLUDED 1
/** LICENSE

    This software's source code, including accompanying documentation and
    demonstration applications, are licensed under the following
    conditions...

    The author (Stephan G. Beal [http://wanderinghorse.net/home/stephan/])
    explicitly disclaims copyright in all jurisdictions which recognize
    such a disclaimer. In such jurisdictions, this software is released
    into the Public Domain.

    In jurisdictions which do not recognize Public Domain property
    (e.g. Germany as of 2011), this software is Copyright (c) 2011
    by Stephan G. Beal, and is released under the terms of the MIT License
    (see below).

    In jurisdictions which recognize Public Domain property, the user of
    this software may choose to accept it either as 1) Public Domain, 2)
    under the conditions of the MIT License (see below), or 3) under the
    terms of dual Public Domain/MIT License conditions described here, as
    they choose.

    The MIT License is about as close to Public Domain as a license can
    get, and is described in clear, concise terms at:

    http://en.wikipedia.org/wiki/MIT_License

    The full text of the MIT License follows:

    --
    Copyright (c) 2011 Stephan G. Beal (http://wanderinghorse.net/home/stephan/)

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

    --END OF MIT LICENSE--

    For purposes of the above license, the term "Software" includes
    documentation and demonstration source code which accompanies
    this software. ("Accompanies" = is contained in the Software's
    primary public source code repository.)

*/

#include <cassert>
#include <stdexcept>
#include "detail/convert_core.hpp"
#include "NativeToJSMap.hpp"
namespace cvv8 {

    /**
       Policy template used by ClassCreator<T> for
       instantiating T objects.
    */
    template <typename T>
    class ClassCreator_Factory
    {
    public:
        typedef T * ReturnType;
        /**
           Must instantiate a new T object based on the given
           arguments. On error it should throw an exception (which the
           binding framework will convert to a JS-side exception). It
           may also return NULL on error, but the error message
           probably won't be as clear for the user.

           Ownership of the object is passed to the caller (the
           binding API internals), and eventually given to v8.

           jsSelf will be the newly-created JS-side 'this' object.  It
           is not normally required by this function but it is
           sometimes useful when we need to bind supplementary
           properties in the ctor, especially when binding a "pure
           C++" class which has no native place to store such
           properties.

           At the time this is called, jsSelf is not connected to the
           native (because it hasn't yet been created).
           Implementations must not perform the actual binding of the
           returned native to jsSelf - ClassCreator will do that
           immediately after Create() returns the new object.
           
           The default implementation simply return (new T).
        */
        static ReturnType Create( v8::Persistent<v8::Object> &, v8::Arguments const & )
        {
            return new T;
        }

        /**
           Must destroy obj using a mechanism complementary to its
           construction via a prior call to Create().

           The default implementation simply calls (delete obj).
        */
        static void Delete( T * obj )
        {
            delete obj;
        }
    };

    /**
       Base class for static ClassCreator options.
     */
    template <typename ValT, ValT Val>
    struct Opt_ConstVal
    {
        typedef ValT Type;
        static const Type Value = Val;
    };

    /**
       Base class for static integer ClassCreator options.
    */
    template <int Val>
    struct Opt_Int : Opt_ConstVal<int,Val>
    {};

    /**
       Base class for static boolean ClassCreator options.
    */
    template <bool Val>
    struct Opt_Bool : Opt_ConstVal<bool,Val>
    {};

    /**
       A ClassCreator policy/option class responsible specifying whether
       or not a ClassCreator-bound class should allow "Foo()" and "new
       Foo()" to behave the same or not. If the Value member is false
       (the default) then "Foo()" is not allowed to behave as a
       constructor call (it will generate an error), otherwise it will
       be treated exactly as if "new Foo()" had been called.
    */
    template <typename T>
    struct ClassCreator_AllowCtorWithoutNew : Opt_Bool<false>
    {};

    /**
       ClassCreator policy which determines whether lookups for native
       types in JS objects should walk through the prototype
       chain. This can decrease the speed of JS-to-this operations and
       is necessary only if bound types will be subclassed (either from
       other bound native types or from JS classes).

       The default value is true for the sake of usability. If JS-side
       subclassing will never be used, you can potentially optimize out a
       few lookup operations by creating the specialization by subclassing
       Opt_Bool<false>.
    */
    template <typename T>
    struct ClassCreator_SearchPrototypeForThis : Opt_Bool<true>
    {};

    /**
        ClassCreator policy type which defines a "type ID" value
        for a type wrapped using ClassCreator. This is used
        together with JSToNative_ObjectWithInternalFieldsTypeSafe
        (type THAT 10 times fast) to provide a lightweight
        (but highly effective) type check when extracting
        natives from v8 (as void pointers). The default
        implementation is fine for all cases i can think of, but i can
        concieve of one or two uses for specializations (e.g. storing the
        JS-side name of the class as the type ID).
        
        The type id must be unique per type except that subtypes may
        (depending on various other binding options) may need use the same
        value as the parent type. If multiple types share the same type ID,
        the type-safety check can be bypassed, _potentially_ leading to an
        illegal static_cast() and subsequent mis-use of the pointer. i
        stress the word "potentially" because to get that condition one
        would have to (A) abuse the object via the C++ API (which doesn't
        happen via the binding process, and you're probably also not going
        to do it) or (B) write some script code to confuse two bound native
        types about who is really who when a particular member is called.

        In the case of subclassed bound types, the
        ClassCreator_TypeID<SubClass> impl should subclass
        ClassCreator_TypeID<ParentClass>. Whether or not this is _required_
        for proper functionality depends at least in part on whether
        (ClassCreator_InternalFields<ParentType>::TypeIDIndex>=0). If it is
        negative, subclasses do not need to explicitly define this policy
        because the type ID won't be used for purposes of validating a JS-held
        pointer's native type.

        TODO: see if we can consolidate this type with TypeName<>. The problem
        at the moment is that JSToNative_ObjectWithInternalFieldsTypeSafe
        takes a (void const * &) and TypeName::Value is a (char const *), which
        won't convert to (void const *) in the context of template parameters.
    */
    template <typename T>
    struct ClassCreator_TypeID
    {
        static const void *Value;
    };

    template <typename T> const void * ClassCreator_TypeID<T>::Value = TypeInfo<T>::TypeName;

    /**
       Convenience base type for ClassCreator_InternalFields
       implementations.

       See the member documentation for the meaning of
       HowMany and Index.

       If any of the following conditions are met then
       a compile-time assertion is triggered:

       - (ObjectIndex<0)

       - (ObjectIndex>=HowMany)

       - (TypeIndex>=HowMany).

       - (TypeIndex == ObjectIndex)

        TypeIndex may be negative, which indicates to ClassCreator that the
        binding should not store type ID information. However, if it is
        negative then T must not be used together with
        JSToNative_ObjectWithInternalFieldsTypeSafe - doing so will trigger
        a compile-time assertion.
    */
    template <typename T, int HowMany = 2, int TypeIndex = 0, int ObjectIndex = 1>
    struct ClassCreator_InternalFields_Base
    {
        /**
           Total number of internal fields assigned to JS-side T
           objects.
        */
        static const int Count = HowMany;

        /**
           The internal field index at which ClassCreator policies should
           expect the native object to be found in any given JS object.
           It must be 0 or greater, and must be less than Value.
        */
        static const int NativeIndex = ObjectIndex;
        
        /**
            The internal field index at which ClassCreator policies
            should expect a type identifier tag to be stored.
            This can be used in conjunction with 
            JSToNative_ObjectWithInternalFieldsTypeSafe (or similar)
            to provide an extra level of type safety at JS runtime.

        */
        static const int TypeIDIndex = TypeIndex;
    private:
        typedef char AssertFields[
            (HowMany > TypeIndex)
            && (HowMany > ObjectIndex)
            && (TypeIndex != ObjectIndex)
            && (ObjectIndex >= 0)
            ? 1 : -1];
    };

    /**
       The ClassCreator policy which sets the number of internal
       fields reserved for JS objects and the internal field index
       (0-based) at which the native object is stored . The Count
       value must be greater than 0 and greater than the NativeIndex
       member. Failing to meet these prerequisites will cause a
       compile-time assertion to be triggered.

       ACHTUNG SUBCLASSERS:

       When using a heirarchy of native types, more than one of which
       is compatible with CastFromJS(), conversions from subtype to 
       base type will fail unless all subtypes use the same internal 
       field placement as the parent type. If this code can detect a 
       mismatch then it will fail gracefully (e.g. a JS-side 
       exception), and if not then it might mis-cast an object and 
       cause Undefined Behaviour.

       If a given parent type uses a custom ClassCreator_InternalFields
       specialization then to ensure that subclasses always have the
       same placement, they "should" define their own policy like
       this:

       @code
       template <>
       struct ClassCreator_InternalFields< SubType >
           : ClassCreator_InternalFields< ParentType >
          {};
       @endcode

       That prohibits special internal field handling in the subtypes,
       but experience hasn't shown that subclasses need their own
       internal fields. Normaly a single internal field is all we need
       when binding native data. And when i say "normally", i mean
       "almost always."

       This must-match requirement is partially a side-effect of the library
       internally using the field count as a santiy check before trying to
       extract data from internal fields. It also exists so that the
       optional (but recommended) type-safety-check support (added in late
       June 2011: see JSToNative_ObjectWithInternalFieldsTypeSafe) will
       treat the subclasses as instances of the base class.
    */
    template <typename T>
    struct ClassCreator_InternalFields : ClassCreator_InternalFields_Base<T>
    {
    };


    /**
        This policy is used by ClassCreator::SetupBindings() as the generic
        interface for plugging in a bound class. Clients are not required to
        specialise this, but see this class' Initialize() for what might
        happen if they don't.
    */
    template <typename T>
    struct ClassCreator_SetupBindings
    {
        /**
            Specializations should perform any class/function-related binding
            here, adding their functionality to the given object (which is
            normally the logical global object but need not be). (Note that the
            handle refererence is const but that object itself can be modified.

            The default implementation throws an exception deriving from
            std::exception, so it must be specialized to be useful. A default
            specialization exists because there are probably a few cases
            out there which don't really need this. But most (if not all)
            need a setup function, and this is the official one for
            ClassCreator-wrapped types. Implementations may of course simply
            forward the call to another, client-provided function.

            On error the binding should throw a NATIVE exception (ideally
            deriving from std::exception because (A) it's portable practice
            and (B) parts of the cvv8 API handles those explicitly).

            Several years of experience have shown that this function (or
            similar implementations) should take some care to make sure
            not to set up their bindings twice. We can do that by using the
            following pattern:

            @code
            typedef ClassCreator<T> CC;
            CC & cc( CC::Instance() );
            if( cc.IsSealed() ) {
                cc.AddClassTo( "T", dest );
                return;
            }
            
            // ... do your bindings here...

            // As the final step:            
            cc.AddClassTo( "T", dest );
            return;
            @endcode
        */
        static void Initialize( v8::Handle<v8::Object> const & target )
        {
            throw std::runtime_error("ClassCreator_SetupBindings<T> MUST be specialized "
                                     "in order to be useful!");
        }
    };

    /**
        A concrete ClassCreator_SetupBindings implementation which forwards
        the call to a user-defined function.
    */
    template <typename T, void (*Func)( v8::Handle<v8::Object> const &) >
    struct ClassCreator_SetupBindings_ClientFunc
    {
        /**
            Calls Func(target).
        */
        static void Initialize( v8::Handle<v8::Object> const & target )
        {
            Func(target);
        }
    };

    /**
       The ClassCreator policy class responsible for doing optional
       class-specific binding-related work as part of the JS/Native
       object construction process.

       The default specialization does nothing (which is okay for the
       general case) but defines the interface which specializations
       must implement.

       Reminder to self: we could arguably benefit by splitting this policy
       into 3 classes, but experience has shown that the metadata used by
       the 3 functions are typically shared amongst the 3 implementations
       (or 2 of them in most cases).
    */
    template <typename T>
    struct ClassCreator_WeakWrap
    {
        typedef typename TypeInfo<T>::NativeHandle NativeHandle;

        /**
           Similar to ), but this is called before the native constructor is called.
           It is rarely needed, but is necessary if one needs to manipulate the JS
           "this" object before the native object is constructed, so that the native ctor
           can access information stored in the JS-side internal fields.

           If this throws a native exception, construction of the
           object will fail and Unwrap() is called, passed
           (jsSelf,NULL), to clean up any data which this function might have
           stored in jsSelf.

           The argv object is the arguments passed to the constructor.

           The default implementation does nothing.
        */
        static void PreWrap( v8::Persistent<v8::Object> const &, v8::Arguments const & )
        {
            return;
        }


        /**
           This operation is called one time from ClassCreator for each
           new object, directly after the native has been connected to
           a Persistent handle.
   
           Note that the ClassCreator code which calls this has already
           taken care of connecting nativeSelf to jsSelf. Client
           specializations of this policy may opt to add their own
           binding mechanisms, e.g. to allow CastToJS<T>() to work.

           Clients should do any bindings-related cleanup in
           Factory::Destruct() or Unwrap(), as appropriate for their
           case.

           Ownership of the objects is unchanged by calling this.

           On error, this function may throw a native exception. If
           that happens, ClassCreator will call
           Unwrap(jsSelf,nativeHandle) and
           Factory<T>::Destruct(nativeSelf) to clean up, and will then
           propagate the exception.

           The default implementation does nothing.
        */
        static void Wrap( v8::Persistent<v8::Object> const &, NativeHandle )
        {
            return;
        }
        
        /**
           This is called from the ClassCreator-generated destructor,
           just before the native destructor is called. If nativeSelf
           is NULL then it means that native construction failed,
           but implementations must (if necessary) clean up any data
           stored in jsSelf by the PreWrap() function.
        
           Specializations may use this to clean up data stored in
           other internal fields of the object (_not_ the field used
           to hold the native itself - that is removed by the
           framework). Optionally, such cleanup may be done in the
           corresponding Factory::Destruct() routine, and must be done
           there if the dtor will need access to such data.

           Note that when this is called, jsSelf and nativeSelf are
           about to be destroyed, so do not do anything crazy with the
           contents of jsSelf and DO NOT destroy nativeSelf (that is
           the job of the ClassCreator_Factory policy).

           Ownership of the objects is unchanged by calling this.

           Unwrap() is called during destruction or when construction
           fails (via a native exception), so any cleanup required for
           the jsSelf object can be delegated to this function, as
           opposed to being performed (and possibly duplicated) in
           PreWrap() and/or Wrap().
           
           The default implementation does nothing.
        */
        static void Unwrap( v8::Handle<v8::Object> const &, NativeHandle )
        {
            return;
        }
    };


#if 0
    namespace Detail
    {
        template <typename Context>
        struct SharedType : public Context
        {
        private:
            SharedType(){}
        public:
            static SharedType & Instance()
            {
                static SharedType bob;
                return bob;
            }
        };
    }
#endif
    /**
       A basic Native-to-JS class binding mechanism. This class does
       not aim to be a monster framework, just something simple,
       mainly for purposes of showing (and testing) what the core
       cvv8 can do.

       The framework must know how to convert JS objects to T objects,
       and for this to work client code must define a JSToNative<T>
       specialization in this manner:

       @code
           template <>
           struct JSToNative<T>
           : JSToNative_ClassCreator<T>
           {};
       @endcode

       If the internal field configuration must be customized then the
       client must define the number of fields by
       specializing/customizing the ClassCreator_InternalFields<T>
       policy class. Additionally, if the client does NOT use the
       above JSToNative<T> implementation then he should create his
       implementation by subclassing
       JSToNative_ObjectWithInternalFields<T,N,M>, where (N,M) are the
       number of internals fields and the index of the field where the
       native object is to be stored. See JSToNative_ClassCreator
       for an example.
       
       TODOs:

       - Certain operations may not work properly when inheriting
       bound classes from JS space, and possibly not even when
       inheriting bound natives from one another. That depends on
       several factors too complex to summarize here.
       
       - See how much of the v8::juice::cw::ClassWrap 
       inheritance-related code we can salvage for re-use here.
       
       - There are known problems when trying to bind inherited methods
       when the parent class has no bound them to JS. i'm not sure how
       i can fix the templates to get this working.
    */

    /**
        Intended to be the base class for JSToNative<T> specializations
        when T is JS-bound using ClassCreator.

        This particular implementation must be defined _after_
        any of the following policies are customized for T:

        - ClassCreator_InternalFields
        - ClassCreator_SearchPrototypeForThis
        - ClassCreator_TypeID (only if TypeSafe is true!)

        If the client will not specialize those types type then the order is
        irrelevant, but when specializing any of them, they must come before
        this JSToNative implementation is instantiated.

        If TypeSafe is true then this type is a proxy for
        JSToNative_ObjectWithInternalFieldsTypeSafe, else it is a proxy for
        JSToNative_ObjectWithInternalFields. Note that ClassCreator is
        hard-wired to implant/deplant type id information if
        ClassCreator_InternalFields<T>::TypeIDIndex is not negative, with the
        _hope_ that JSToNative<T> will use it, but it does not enforce that
        the type ID is used. For types where the internal fields' TypeIDIndex
        is negative, ClassCreator will not set up bits for the type check,
        which means a slightly smaller runtime memory footprint.
    */
    template <typename T, bool TypeSafe = ClassCreator_InternalFields<T>::TypeIDIndex >= 0 >
    struct JSToNative_ClassCreator :
        XIfElse< TypeSafe,
            JSToNativeObjectWithInternalFieldsTypeSafe<T,
                                            ClassCreator_TypeID<T>::Value,
                                            ClassCreator_InternalFields<T>::Count,
                                            ClassCreator_InternalFields<T>::TypeIDIndex,
                                            ClassCreator_InternalFields<T>::NativeIndex,
                                            ClassCreator_SearchPrototypeForThis<T>::Value
                                            >,
            JSToNativeObjectWithInternalFields<T,
                                            ClassCreator_InternalFields<T>::Count,
                                            ClassCreator_InternalFields<T>::NativeIndex,
                                            ClassCreator_SearchPrototypeForThis<T>::Value
                                            >            
        >::Type
    {
    };

#if 0
    //! Experimental.
    template <typename ParentT, typename SubT >
    struct JSToNative_ClassCreator_Subclass
    {
        typedef typename TypeInfo<SubT>::NativeHandle ResultType;
        ResultType operator()( v8::Handle<v8::Value> const & h ) const
        {
            typedef typename TypeInfo<ParentT>::NativeHandle PTP;
            PTP typeCheck; typeCheck = (ResultType)NULL
                /* If compiler errors led you here then SubT probably does not
                    publicly subclass ParentT. */
                ;
            PTP p = CastFromJS<ParentT>(h);
            //std::cerr << "dyncast="<<dynamic_cast<ResultType>(p)<<"\n";
            return p ? dynamic_cast<ResultType>(p) : NULL; 
        }
    };
#endif

#if !defined(DOXYGEN)
    namespace Detail
    {
        /**
           A base class for ClassCreator_Factory_CtorArityDispatcher.
           We don't really need this level of indirection, i think.
        */
        template <typename T>
        struct Factory_CtorForwarder_Base
        {
            typedef typename TypeInfo<T>::Type Type;
            typedef typename TypeInfo<T>::NativeHandle NativeHandle;
            static void Delete( NativeHandle nself )
            {
                delete nself;
            }
        protected:
            /**
               If argv.Length() >= Arity then this function ignores errmsg and
               returns true, otherwise it writes a descriptive error message
               to errmsg and return false.
            */
            static bool argv_check( v8::Arguments const & argv, int Arity )
            {
                if( argv.Length() >= Arity ) return true;
                else
                {
                    StringBuffer msg;
                    msg << "constructor requires " << Arity << " arguments!";
                    throw std::range_error(msg.Content().c_str());
                    return false;
                }
            }
        };
    }
#endif // !DOXYGEN

    /**
        Can be used as a concrete ClassCreator_Factor<T> 
        specialization to forward JS ctor calls directly to native 
        ctors.
    
        T must be the ClassCreator'd type to construct. CtorProxy must
        be a type having this interface:
        
        @code
        TypeInfo<T>::NativeHandle Call( v8::Arguments const & );
        @endcode

        Normally CtorProxy would be CtorForwarder or CtorArityDispatcher,
        but any interface-compatible type will do.

        It must return a new object instance on success. On error it
        may return NULL and "should" throw a native exception explaining
        the problem. The exception will be caught by ClassCreator and
        transformed into a JS-side exception.

        If CtorProxy::Call() succeeds (returns non-NULL and does not throw)
        then NativeToJSMap<T> is used to create a native-to-JS mapping.
        To make use of this, the client should do the following:
        
        @code
        // in the cvv8 namespace:
        template <>
            struct NativeToJS<T> : NativeToJSMap<T>::NativeToJSImpl {};
        @endcode
        
        After that, CastToJS<T>( theNativeObject ) can work.
        
        The mapping is cleaned up when (if!) the object is sent through
        the JS garbage collector or the client somehow triggers its
        JS-aware destruction (e.g. via ClassCreator::DestroyObject(),
        assuming the type was wrapped using ClassCreator).
    */
    template <typename T, typename CtorProxy>
    struct ClassCreator_Factory_NativeToJSMap : Detail::Factory_CtorForwarder_Base<T>
    {
    public:
        typedef NativeToJSMap<T> N2JMap;
        typedef typename TypeInfo<T>::Type Type;
        typedef typename TypeInfo<T>::NativeHandle NativeHandle;
        
        /**
            If CtorProxy::Call(argv) succeeds, N2JMap::Insert(jself, theNative)
            is called. The result of CtorProxy::Call() is returned.
        */
        static NativeHandle Create( v8::Persistent<v8::Object> jself, v8::Arguments const &  argv )
        {
            NativeHandle n = CtorProxy::Call( argv );
            if( n ) N2JMap::Insert( jself, n );
            return n;
        }
        /**
            Calls N2JMap::Remove( nself ) then (delete nself).
        */
        static void Delete( NativeHandle nself )
        {
            N2JMap::Remove( nself );
            delete nself;
        }
    };

    /** @deprecated Use ClassCreator_Factory_Dispatcher instead (same interface).
    */
    template <typename T,typename CtorForwarderList>
    struct ClassCreator_Factory_CtorArityDispatcher : Detail::Factory_CtorForwarder_Base<T>
    {
    public:
        typedef typename TypeInfo<T>::Type Type;
        typedef typename TypeInfo<T>::NativeHandle NativeHandle;
        static NativeHandle Create( v8::Persistent<v8::Object> , v8::Arguments const &  argv )
        {
            typedef CtorArityDispatcher<CtorForwarderList> Proxy;
            return Proxy::Call( argv );
        }
    };

    /**
        A ClassCreator_Factory implementation which forwards its Create()
        member to CtorT::Call() (the interface used by CtorForwarder and friends).

        T must (or is assumed to) be a ClassCreator<T>-wrapped class.
        CtorForwarderList must be a Signature typelist of CtorForwarder
        types and its "return type" must be T (optionally pointer-qualified).
        
        Example:
        
        @code
        typedef CtorForwarder<MyType *()> C0;
        typedef CtorForwarder<MyType *(int)> C1;
        typedef CtorForwarder<MyType *(int, double)> C2;
        typedef Signature< CFT (C0, C1, C2) > CtorList;
        
        // Then create Factory specialization based on those:
        template <>
        struct ClassCreator_Factory<MyType> : 
            ClassCreator_Factory_Dispatcher<MyType, CtorArityDispatcher<CtorList> > {};
        @endcode

        Or:
        
        @code
        template <>
        struct ClassCreator_Factory<MyType> :
            ClassCreator_Factory_Dispatcher< MyType, CtorForwarder<MyType *()> >
        {};
        @endcode
    */
    template <typename T,typename CtorT>
    struct ClassCreator_Factory_Dispatcher : Detail::Factory_CtorForwarder_Base<T>
    {
    public:
        typedef typename TypeInfo<T>::Type Type;
        typedef typename TypeInfo<T>::NativeHandle NativeHandle;
        static NativeHandle Create( v8::Persistent<v8::Object>, v8::Arguments const &  argv )
        {
            return CtorT::Call( argv );
        }
    };

    /**
       A special-case factory implementation for use when T
       is abstract or otherwise should not be instantiable
       from JS code. It has one or two obscure uses when binding
       certain class hierarchies.
    */
    template <typename T>
    struct ClassCreator_Factory_Abstract : Detail::Factory_CtorForwarder_Base<T>
    {
    public:
        typedef typename TypeInfo<T>::Type Type;
        typedef typename TypeInfo<T>::NativeHandle NativeHandle;
        /**
           Always returns NULL.
        */
        static NativeHandle Create( v8::Persistent<v8::Object> jself, v8::Arguments const &  argv )
        {
            return NULL;
        }
    };

}// namespaces

#endif /* CODE_GOOGLE_COM_P_V8_CONVERT_CLASS_CREATOR_HPP_INCLUDED */
