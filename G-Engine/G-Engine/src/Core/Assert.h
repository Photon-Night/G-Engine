#pragma once

#ifdef GE_DEBUG
#define GE_ENABLE_ASSERTS
#endif

#ifdef GE_ENABLE_ASSERTS
#define GE_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { GE_ERROR("Assertion Failed"); __debugbreak(); } }
#define GE_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { GE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#define GE_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro
#define GE_GET_ASSERT_MACRO(...) GE_EXPAND_VARGS(GE_ASSERT_RESOLVE(__VA_ARGS__, GE_ASSERT_MESSAGE, GE_ASSERT_NO_MESSAGE))

#define GE_ASSERT(...) GE_EXPAND_VARGS( GE_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#define GE_CORE_ASSERT(...) GE_EXPAND_VARGS( GE_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#else
#define GE_ASSERT(...)
#define GE_CORE_ASSERT(...)
#define GE_ASSERT_NO_MESSAGE(...)
#define GE_ASSERT_MESSAGE(...)
#endif