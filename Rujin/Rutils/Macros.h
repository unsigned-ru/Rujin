#pragma once

#pragma region Error Checking
#define ASSERT_IF_(isTrue)\
	if(isTrue){HANDLE_ERROR("[ASSERT]" #isTrue);}

#define ASSERT_IF(isTrue, ...)\
	if(isTrue){HANDLE_ERROR(__VA_ARGS__);}

#define ASSERT_NULL_(object)\
	if((object) == nullptr){HANDLE_ERROR("[ASSERT]" #object " == nullptr");}

#define ASSERT_NULL(object, ...)\
	if((object) == nullptr){HANDLE_ERROR(__VA_ARGS__);}

#define HANDLE_ERROR(...)\
	Logger::LogError(LOG_STRING(__VA_ARGS__))
#pragma endregion


#define SafeDelete(pObject)\
	if(pObject != nullptr)\
	{\
		delete pObject;\
		pObject = nullptr;\
	}


#define STREAM_STRING(text) static_cast<std::stringstream&&>(std::stringstream{} << (text)).str()