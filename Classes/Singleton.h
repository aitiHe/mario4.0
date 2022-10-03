//#ifndef _SINGLETON_H_
//#define _SINGLETON_H_
//
//#include <memory>
//using std::tr1::shared_ptr;
//
//template<class T>
//class Singleton
//{
//public:
//	static inline T* getInstance();
//
//protected:
//	Singleton(){}
//	virtual ~Singleton(){}
//	static shared_ptr<T> _instance;
//};
//
//template<class T>
//inline T* Singleton<T>::getInstance()
//{
//	if (_instance->get() == NULL)
//	{
//		_instance.reset(new T);
//	}
//	return _instance->get();
//}
//
//#define DECLARE_SINGLETON_MEMBER(_Ty)   \
//	template<> shared_ptr<_Ty> Singleton<_Ty>::_instance;
//
//#endif