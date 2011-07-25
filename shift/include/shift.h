#ifndef SHIFT_H
#define SHIFT_H


#include <baseobject.h>
#include <xlptr.h>
#include <vector>

namespace XL
{
#define SLOT(a1, a2) a1, &a2
struct none{};
// Forward decls
class Object;
template <class T,class T2, class T3,class T4,class T5,class T6,class T7>
class Signal;
template <class T,class T2, class T3,class T4,class T5,class T6,class T7>
class BlockedSignal;

class BaseSlot : public BaseObject
{
public:
    virtual void (BaseObject::*method0()) () = 0;
    virtual void emit() = 0;
    virtual void setSender(Object*) = 0;
};

// Functor //
template <class T,class T2, class T3,class T4,class T5,class T6,class T7>
class Slot: public BaseSlot
{
    template <class X, class X2, class X3, class X4, class X5, class X6,
              class X7> friend class Slot;
public:

    typedef void (BaseObject::*fn)();
    typedef void (BaseObject::*fn1)(T);
    typedef void (BaseObject::*fn2)(T,T2);
    typedef void (BaseObject::*fn3)(T,T2,T3);
    typedef void (BaseObject::*fn4)(T,T2,T3,T4);
    typedef void (BaseObject::*fn5)(T,T2,T3,T4,T5);
    typedef void (BaseObject::*fn6)(T,T2,T3,T4,T5,T6);
    typedef void (BaseObject::*fn7)(T,T2,T3,T4,T5,T6,T7);

    typedef Signal<T,T2,T3,T4,T5,T6,T7> TSignal;

    enum Type {
        TypeNoArg,
        TypeArg1,
        TypeArg2,
        TypeArg3,
        TypeArg4,
        TypeArg5,
        TypeArg6,
        TypeArg7
    };

    /* Make sure the receiver informs calling object before its going to be
     * destroyed.
     */
    void init(BaseObject * obj);

    Slot(BaseObject * obj, fn f): slotCopy(0), _type(TypeNoArg) {
        init(obj);
        _method.mf = f;
    }

    Slot(BaseObject * obj, fn1 f): slotCopy(0), _type(TypeArg1) {
        init(obj);
        _method.mfn = f;
    }

    Slot(BaseObject* obj, fn2 f): slotCopy(0), _type(TypeArg2) {
        init(obj);
        _method.mfn2 = f;
    }

    Slot(BaseObject* obj, fn3 f): slotCopy(0), _type(TypeArg3) {
        init(obj);
        _method.mfn3 = f;
    }

    Slot(BaseObject* obj, fn4 f): slotCopy(0), _type(TypeArg4) {
        init(obj);
        _method.mfn4 = f;
    }

    Slot(BaseObject* obj, fn5 f): slotCopy(0), _type(TypeArg5) {
        init(obj);
        _method.mfn5 = f;
    }

    Slot(BaseObject* obj, fn6 f): slotCopy(0), _type(TypeArg6) {
        init(obj);
        _method.mfn6 = f;
    }

    Slot(BaseObject* obj, fn7 f): slotCopy(0), _type(TypeArg7) {
        init(obj);
        _method.mfn7 = f;
    }

    void emit ()
    {
        if(!isActive())
            return;
        if(_type == TypeNoArg) {
            obj->setSender(parentSender);
            (obj->*((fn)_method.mf))();
            //obj->setSender(0);
        }
    }

    void emit ( T arg )
    {
        if(!isActive())
            return;
        if(_type == TypeNoArg)
            {
                emit();
                return;
            }
        if(_type == TypeArg1) {
            obj->setSender(parentSender);
            (obj->*((fn1)_method.mfn))(arg);
            obj->setSender(0);
        }
    }

    void emit ( T arg, T2 a2 )
    {
        if(!isActive())
            return;
        if(_type == TypeNoArg)
            {
                emit();
                return;
            }
        if(_type == TypeArg1)
            {
                emit(arg);
                return;
            }
        if(_type == TypeArg2) {
            obj->setSender(parentSender);
            (obj->*((fn2)_method.mfn2))(arg,a2);
            obj->setSender(0);
        }
    }

    void emit ( T arg , T2 a2, T3 a3)
    {
        if(!isActive())
            return;
        if(_type == TypeNoArg)
            {
                emit();
                return;
            }
        if(_type == TypeArg1)
            {
                emit(arg);
                return;
            }
        if(_type == TypeArg2)
            {
                emit(arg,a2);
                return;
            }
        if(_type == TypeArg3) {
            obj->setSender(parentSender);
            (obj->*((fn3)_method.mfn3))(arg,a2,a3);
            obj->setSender(0);
        }
    }

    void emit ( T arg, T2 a2, T3 a3, T4 a4)
    {
        if(!isActive())
            return;
        if(_type == TypeNoArg)
            {
                emit();
                return;
            }
        if(_type == TypeArg1)
            {
                emit(arg);
                return;
            }
        if(_type == TypeArg2)
            {
                emit(arg,a2);
                return;
            }
        if(_type == TypeArg3)
            {
                emit(arg,a2,a3);
                return;
            }
        if(_type == TypeArg4) {
            obj->setSender(parentSender);
            (obj->*((fn4)_method.mfn4))(arg,a2,a3,a4);
            obj->setSender(0);
        }
    }

    void emit ( T arg, T2 a2, T3 a3,T4 a4,T5 a5)
    {
        if(!isActive())
            return;
        if(_type == TypeNoArg)
            {
                emit();
                return;
            }
        if(_type == TypeArg1)
            {
                emit(arg);
                return;
            }
        if(_type == TypeArg2)
            {
                emit(arg,a2);
                return;
            }
        if(_type == TypeArg3)
            {
                emit(arg,a2,a3);
                return;
            }
        if(_type == TypeArg4)
            {
                emit(arg,a2,a3,a4);
                return;
            }
        if(_type == TypeArg5) {
            obj->setSender(parentSender);
            (obj->*((fn5)_method.mfn5))(arg,a2,a3,a4,a5);
            obj->setSender(0);
        }
    }

    void emit ( T arg , T2 a2, T3 a3,T4 a4,T5 a5,T6 a6)
    {
        if(!isActive())
            return;
        if(_type == TypeNoArg)
            {
                emit();
                return;
            }
        if(_type == TypeArg1)
            {
                emit(arg);
                return;
            }
        if(_type == TypeArg2)
            {
                emit(arg,a2);
                return;
            }
        if(_type == TypeArg3)
            {
                emit(arg,a2,a3);
                return;
            }
        if(_type == TypeArg4)
            {
                emit(arg,a2,a3,a4);
                return;
            }
        if(_type == TypeArg5)
            {
                emit(arg,a2,a3,a4,a5);
                return;
            }
        if(_type == TypeArg6) {
            obj->setSender(parentSender);
            (obj->*((fn6)_method.mfn6))(arg,a2,a3,a4,a5,a6);
            obj->setSender(0);
        }
    }

    void emit ( T arg , T2 a2, T3 a3,T4 a4,T5 a5,T6 a6,T7 a7)
    {
        if(!isActive())
            return;
        if(_type == TypeNoArg)
            {
                emit();
                return;
            }
        if(_type == TypeArg1)
            {
                emit(arg);
                return;
            }
        if(_type == TypeArg2)
            {
                emit(arg,a2);
                return;
            }
        if(_type == TypeArg3)
            {
                emit(arg,a2,a3);
                return;
            }
        if(_type == TypeArg4)
            {
                emit(arg,a2,a3,a4);
                return;
            }
        if(_type == TypeArg5)
            {
                emit(arg,a2,a3,a4,a5);
                return;
            }
        if(_type == TypeArg6)
            {
                emit(arg,a2,a3,a4,a5,a6);
                return;
            }
        if(_type == TypeArg7) {
            obj->setSender(parentSender);
            (obj->*((fn7) _method.mfn7))(arg,a2,a3,a4,a5,a6,a7);
            obj->setSender(0);
        }
    }

    void setSender(Object* obj) { parentSender = obj; }

    BaseObject* receiver() const { return obj; }

    void cloneInternally(const XLPtr< BaseSlot > &x ) {
        if(subSlot)
            subSlot->slotCopy = x;
    }

    XLPtr<BaseSlot>& getInternalClone() {
        if(subSlot)
           return subSlot->slotCopy;
        return slotCopy;
    }

    bool isActive() const { return active; }

    virtual void (BaseObject::*method0()) () {
        if(_type == TypeNoArg)
            return _method.mf;
        return 0;
    }

 private:
    // deactivate slot
    void deactivate() { active = false; }
    bool isubslot;
    bool active;

    Slot<none,none,none,none,none,none,none> *subSlot;
    XLPtr<BaseSlot> slotCopy;

    BaseObject* obj;
    Type _type;

    union {
        fn  mf;
        fn1 mfn;
        fn2 mfn2;
        fn3 mfn3;
        fn4 mfn4;
        fn5 mfn5;
        fn6 mfn6;
        fn7 mfn7;
    } _method;

    Object* parentSender;
};


// A signal is a collection of slots.  When emitted, it executes all connected
// slots immediately.
template <class T = none, class T2 = none, class T3 = none,
          class T4 = none,class T5 = none, class T6 = none, class T7 = none>
class Signal
{
    template <class X, class X2, class X3, class X4, class X5, class X6,
              class X7> friend class Slot;
 public:
    typedef Slot<T,T2,T3,T4,T5,T6,T7> slot_arg;
    typedef BlockedSignal<T,T2,T3,T4,T5,T6,T7> BSignal;
    typedef std::vector< XLPtr<BaseSlot > > SlotList;

    Signal(): _sender(0) { }
    virtual ~Signal() { }

    template <class _Obj>
    void connect( BaseObject* obj, void (_Obj::*f)()) {
        slot_arg *memfn = createSlot(obj, static_cast<typename slot_arg::fn>(f));
        insertSlot(memfn);
    }

    template <class _Obj>
    void connect( BaseObject* obj, void (_Obj::*f)(T)) {
        slot_arg *memfn = createSlot(obj, static_cast<typename slot_arg::fn1>(f));
        insertSlot(memfn);
    }

    template <class _Obj>
    void connect( BaseObject* obj, void (_Obj::*f)(T,T2)) {
        slot_arg *memfn = createSlot(obj, static_cast<typename slot_arg::fn2>(f));
        insertSlot(memfn);
    }

    template <class _Obj>
    void connect( BaseObject* obj, void (_Obj::*f)(T,T2,T3)) {
        slot_arg *memfn = createSlot(obj, static_cast<typename slot_arg::fn3>(f));
        insertSlot(memfn);
    }

    template <class _Obj>
    void connect( BaseObject* obj, void (_Obj::*f)(T,T2,T3,T4)) {
        slot_arg *memfn = createSlot(obj, static_cast<typename slot_arg::fn4>(f));
        insertSlot(memfn);
    }

    template <class _Obj>
    void connect( BaseObject* obj, void (_Obj::*f)(T,T2,T3,T4,T5)) {
        slot_arg *memfn = createSlot(obj, static_cast<typename slot_arg::fn5>(f));
        insertSlot(memfn);
    }

    template <class _Obj>
    void connect( BaseObject* obj, void (_Obj::*f)(T,T2,T3,T4,T5,T6)) {
        slot_arg *memfn = createSlot(obj, static_cast<typename slot_arg::fn6>(f));
        insertSlot(memfn);
    }

    template <class _Obj>
    void connect( BaseObject* obj, void (_Obj::*f)(T,T2,T3,T4,T5,T6,T7)) {
        slot_arg *memfn = createSlot(obj, static_cast<typename slot_arg::fn7>(f));
        insertSlot(memfn);
    }

    // Connect to another signal. The other signal is emitted immediately
    // whenever this signal is emitted
    void connect(Signal<T,T2,T3,T4,T5,T6,T7> &sig) {
        sp.insert(sp.end(), sig.sp.begin(), sig.sp.end());
    }

    bool isActive() const { return !sp.empty(); }

    BSignal operator() () { return emit(false); }
    BSignal operator() (T a ) { return emit(a,false); }
    BSignal operator() (T a,T2 b) { return emit(a,b,false); }
    BSignal operator() (T a,T2 b ,T3 c) { return emit(a,b,c,false); }
    BSignal operator() (T a,T2 b ,T3 c,T4 d) { return emit(a,b,c,d,false); }
    BSignal operator() (T a,T2 b ,T3 c,T4 d,T5 e) { return emit(a,b,c,d,e,
                                                                false); }
    BSignal operator() (T a,T2 b ,T3 c,T4 d,T5 e,T6 f) {
        return emit(a,b,c,d,e,f,false); }
    BSignal operator() (T a,T2 b ,T3 c,T4 d,T5 e,T6 f,T7 g) {
        return emit(a,b,c,d,e,f,g,false);
    }

    BSignal emit(bool autoexec = true);
    BSignal emit(T a,bool autoexec = true);
    BSignal emit(T a,T2 b,bool autoexec = true);
    BSignal emit(T a,T2 b ,T3 c,bool autoexec = true);
    BSignal emit(T a,T2 b ,T3 c,T4 d,bool autoexec = true);
    BSignal emit(T a,T2 b ,T3 c,T4 d,T5 e,bool autoexec = true);
    BSignal emit(T a,T2 b ,T3 c,T4 d,T5 e,T6 f,bool autoexec = true);
    BSignal emit(T a,T2 b ,T3 c,T4 d,T5 e,T6 f,T7 g,bool autoexec = true);

    void setSender(Object* object) { _sender = object; }
    Object* sender() const { return _sender; }

 protected:
    virtual slot_arg * createSlot( BaseObject* obj, void (BaseObject::*f)()) {
        return new slot_arg(obj, f);
    }

    virtual slot_arg * createSlot( BaseObject* obj,
                                   void (BaseObject::*f)(T)) {
        return new slot_arg(obj,f);
    }

    virtual slot_arg * createSlot( BaseObject* obj,
                                   void (BaseObject::*f)(T,T2)) {
        return new slot_arg(obj, f);
    }

    virtual slot_arg *createSlot( BaseObject* obj,
                                  void (BaseObject::*f)(T,T2,T3)) {
        return new slot_arg(obj, f);
    }

    virtual slot_arg *createSlot( BaseObject* obj,
                                  void (BaseObject::*f)(T,T2,T3,T4)) {
        return new slot_arg(obj, f);
    }

    virtual slot_arg *createSlot( BaseObject* obj,
                                  void (BaseObject::*f)(T,T2,T3,T4,T5)) {
        return new slot_arg(obj, f);
    }

    virtual slot_arg *createSlot( BaseObject* obj,
                                  void (BaseObject::*f)(T,T2,T3,T4,T5,T6)) {
        return new slot_arg(obj, f);
    }

    virtual slot_arg *createSlot( BaseObject* obj,
                                  void (BaseObject::*f)(T,T2,T3,T4,T5,T6,T7)) {
        return new slot_arg(obj, f);
    }

    virtual void insertSlot( slot_arg * memfn ) {
        XLPtr<BaseSlot> s = memfn;
        // store a copy of itself to the subslot
        memfn->cloneInternally(s);
        sp.push_back(s);
    }

    SlotList* slotList() const { return (SlotList*)&sp; }

 private:
    void destroyedSlot( slot_arg * memfn)  {
        XLPtr<BaseSlot> s = memfn;
        sp.push_back(s);
    }

    SlotList sp;
    Object* _sender;
};

// Signal encapsulator for deferred emission
// Store the argument as a single instance. Store the list of slots
template <class T = none, class T2 = none, class T3 = none,
          class T4 = none,class T5 = none, class T6 = none, class T7 = none>
class BlockedSignal
{
public:
    typedef Signal<T,T2,T3,T4,T5,T6,T7> TSignal;

    BlockedSignal( TSignal* signal, T a,T2 a2,T3 a3,T4 a4,T5 a5,
                  T6 a6,T7 a7)
        :mainSignal(signal),
         arg1(a),
         arg2(a2),
         arg3(a3),
         arg4(a4),
         arg5(a5),
         arg6(a6),
         arg7(a7) { }
    void setSender(Object* object)
    {
        if(mainSignal)
            mainSignal->setSender(object);
    }

    void play()
    {
        if(mainSignal)
            mainSignal->emit(arg1,arg2,arg3,arg4,arg5,arg6,arg7);
    }
private:
    TSignal *mainSignal;
    T arg1;
    T2 arg2;
    T3 arg3;
    T4 arg4;
    T5 arg5;
    T6 arg6;
    T7 arg7;
};

// Specialization for people who want Signal<void> instead of Signal<>
template<>
class Signal<void>: public Signal<>
{ };

template <class T,class T2, class T3,class T4,class T5,class T6,class T7>
inline BlockedSignal<T,T2,T3,T4,T5,T6,T7>
Signal<T,T2,T3,T4,T5,T6,T7>::emit(bool autoexec)
{

    if(autoexec) {
        size_t sz = sp.size();
        for(unsigned int i=0; i<sz; i++) {

            BaseSlot* b = sp[i];

            slot_arg* s = (slot_arg *) b;
            s->setSender(_sender);
            s->emit();
        }
    }
    return BSignal(this,none(),none(),none(),none(),none(),none(),none());
}

template <class T,class T2, class T3,class T4,class T5,class T6,class T7>
inline BlockedSignal<T,T2,T3,T4,T5,T6,T7>
Signal<T,T2,T3,T4,T5,T6,T7>::emit(T a,bool autoexec)
{
    if(autoexec) {
        size_t sz = sp.size();
        for(unsigned int i=0; i<sz; i++) {

            BaseSlot* b = sp[i];

            slot_arg* s = (slot_arg *) b;
            s->setSender(_sender);
            s->emit(a);
        }
    }
    return BSignal(this,a,none(),none(),none(),none(),none(),none());
}

template <class T,class T2,class T3,class T4,class T5,class T6,class T7>
inline BlockedSignal<T,T2,T3,T4,T5,T6,T7>
Signal<T,T2,T3,T4,T5,T6,T7>::emit(T a,T2 a2,bool autoexec)
{
    if(autoexec) {
        size_t sz = sp.size();
        for(unsigned int i=0; i<sz; i++) {

            BaseSlot* b = sp[i];

            slot_arg* s = (slot_arg *) b;

            s->setSender(_sender);
            s->emit(a,a2);
        }
    }
    return BSignal(this,a,a2,none(),none(),none(),none(),none());
}

template <class T,class T2,class T3,class T4,class T5,class T6,class T7>
inline BlockedSignal<T,T2,T3,T4,T5,T6,T7>
Signal<T,T2,T3,T4,T5,T6,T7>::emit(T a,T2 a2,T3 a3,bool autoexec)
{
    if(autoexec) {
        size_t sz = sp.size();
        for(unsigned int i=0; i<sz; i++) {

            BaseSlot* b = sp[i];

            slot_arg* s = (slot_arg *) b;

            s->setSender(_sender);
            s->emit(a,a2,a3);
        }
    }
    return BSignal(this,a,a2,a3,none(),none(),none(),none());
}

template <class T,class T2,class T3,class T4,class T5,class T6,class T7>
inline BlockedSignal<T,T2,T3,T4,T5,T6,T7>
Signal<T,T2,T3,T4,T5,T6,T7>::emit(T a,T2 a2,T3 a3,T4 a4,bool autoexec)
{
    if(autoexec) {
        size_t sz = sp.size();
        for(unsigned int i=0; i<sz; i++) {

            BaseSlot* b = sp[i];

            slot_arg* s = (slot_arg *) b;

            s->setSender(_sender);
            s->emit(a,a2,a3,a4);
        }
    }
    return BSignal(this,a,a2,a3,a4,none(),none(),none());
}

template <class T,class T2,class T3,class T4,class T5,class T6,class T7>
inline BlockedSignal<T,T2,T3,T4,T5,T6,T7>
Signal<T,T2,T3,T4,T5,T6,T7>::emit(T a,T2 a2,T3 a3,T4 a4,T5 a5,bool autoexec)
{
    if(autoexec) {
        size_t sz = sp.size();
        for(unsigned int i=0; i<sz; i++) {

            BaseSlot* b = sp[i];

            slot_arg* s = (slot_arg *) b;

            s->setSender(_sender);
            s->emit(a,a2,a3,a4,a5);
        }
    }
    return BSignal(this,a,a2,a3,a4,a5,none(),none());
}

template <class T,class T2,class T3,class T4,class T5,class T6,class T7>
inline BlockedSignal<T,T2,T3,T4,T5,T6,T7>
Signal<T,T2,T3,T4,T5,T6,T7>::emit(T a,T2 a2,T3 a3,T4 a4,T5 a5,
                                  T6 a6,bool autoexec)
{
    if(autoexec) {
        size_t sz = sp.size();
        for(unsigned int i=0; i<sz; i++) {

            BaseSlot* b = sp[i];

            slot_arg* s = (slot_arg *) b;

            s->setSender(_sender);
            s->emit(a,a2,a3,a4,a5,a6);
        }
    }
    return BSignal(this,a,a2,a3,a4,a5,a6,none());
}

template <class T,class T2,class T3,class T4,class T5,class T6,class T7>
inline BlockedSignal<T,T2,T3,T4,T5,T6,T7>
Signal<T,T2,T3,T4,T5,T6,T7>::emit(T a,T2 a2,T3 a3,T4 a4,T5 a5,
                                  T6 a6,T7 a7,bool autoexec)
{
    if(autoexec) {
        size_t sz = sp.size();
        for(unsigned int i=0; i<sz; i++) {

            BaseSlot* b = sp[i];

            slot_arg* s = (slot_arg *) b;

            s->setSender(_sender);
            s->emit(a,a2,a3,a4,a5,a6,a7);
        }
    }
    return BSignal(this,a,a2,a3,a4,a5,a6,a7);
}

}
#include "xlslot.hpp"

#endif // SHIFT_H
