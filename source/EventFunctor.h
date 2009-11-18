#ifndef EVENTFUNCTOR_H
#define EVENTFUNCTOR_H

class GameEvent;

class EventFunctor {
public:
	virtual void operator()(GameEvent* evnt) const = 0;
	virtual void call(GameEvent* evnt) const = 0;
	virtual const void* objectPtr() const = 0;
};

template <class T> class TEventFunctor : public EventFunctor {
public:
	TEventFunctor();
	//object pointer, function pointer
	TEventFunctor(T *tObject, void(T::*funcPtr)(GameEvent*));

	//comparison operator
	bool operator == (TEventFunctor &rhs);

	//override function call operator
	virtual void operator()(GameEvent *evnt) const;
	//alternate to function call operator
	virtual void call(GameEvent *evnt) const;

	const void* objectPtr() const;

private:
	T *mTObject;					//pointer to the object
	void (T::*mFuncPtr)(GameEvent*);	//pointer to the member function
};

template <class T>
TEventFunctor<T>::TEventFunctor(T *tObject, void(T::*funcPtr)(GameEvent*)) {
	mTObject = tObject;
	mFuncPtr = funcPtr;
}

template <class T>
bool TEventFunctor<T>::operator ==(TEventFunctor<T> &rhs) {
	if(mFuncPtr == rhs.mFuncPtr &&
		mTObject == rhs.mTObject) {
			return true;
	}

	return false;
}

template <class T>
void TEventFunctor<T>::operator() (GameEvent *evnt) const {
	(mTObject->*mFuncPtr)(evnt);
}

template <class T>
void TEventFunctor<T>::call(GameEvent *evnt) const {
	(mTObject->*mFuncPtr)(evnt);
}

template <class T>
const void* TEventFunctor<T>::objectPtr() const {
	return mTObject;
}

#endif