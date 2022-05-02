#pragma once

#pragma region Error Checking
#define ASSERT_IF(isTrue)\
	if(isTrue){LOG_ERROR("[ASSERT]" #isTrue);}

#define ASSERT_NULL(object)\
	if((object) == nullptr){LOG_ERROR("[ASSERT]" #object " == nullptr");}

#pragma endregion


#define SafeDelete(pObject)\
	if(pObject != nullptr)\
	{\
		delete pObject;\
		pObject = nullptr;\
	}


#define STREAM_STRING(text) static_cast<std::stringstream&&>(std::stringstream{} << (text)).str()