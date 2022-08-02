#ifndef MACROS_H
#define MACROS_H


#pragma region Error Checking
#define ASSERT_IF(isTrue)\
	if(isTrue){LOG_ERROR("[ASSERT]" #isTrue);}


#define ASSERT_IF_(isTrue, log)\
	if(isTrue){LOG_ERROR("[ASSERT]" #isTrue ": " log);}


#define ASSERT_NULL(object)\
	if((object) == nullptr){LOG_ERROR("[ASSERT] " #object " == nullptr");}

#define ASSERT_NULL_(object, log)\
	if((object) == nullptr){LOG_ERROR("[ASSERT] " log);}

#pragma endregion

#define SAFE_DELETE(ptr)\
		delete (ptr);\
		(ptr) = nullptr;

#ifdef PX_PHYSX_COMMON_API
#define PX_SAFE_RELEASE(pInterface)\
	if(pInterface != nullptr)\
	{\
		pInterface->release();\
		pInterface = nullptr;\
	}

#endif

#define STREAM_STRING(text) static_cast<std::stringstream&&>(std::stringstream{} << (text)).str()


#endif // Include Guard: MACROS_H
