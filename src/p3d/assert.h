#pragma once

#if defined(P3D_ASSERT_ENABLE_DEBUG_BREAK) && defined(P3D_DEBUG)
    #if defined(_MSC_VER) || defined(_MSC_FULL_VER) 
        #define _P3D_ASSERT_DEBUG_BREAK __debugbreak();
    #else
    //TODO: support for other platforms
    #endif
#else
    #define _P3D_ASSERT_DEBUG_BREAK ((void)(0));
#endif

#ifdef P3D_ASSERT_ENABLE_ERROR_PRINT
    #include <iostream>
    #define _P3D_ASSERT_ERROR_PRINT(msg) do {\
        std::cout << msg <<std::endl;\
    } while(0);
#else
    #define _P3D_ASSERT_ERROR_PRINT(msg) ((void)(0));
#endif

//This assert is meant to be used inside functions that return a boolean to indicate success/failure
#define P3D_ASSERT_R(expr, msg)  do { \
        if(!(expr)) { \
            _P3D_ASSERT_ERROR_PRINT(msg)\
            _P3D_ASSERT_DEBUG_BREAK\
            return false;\
        }\
    } while(0)

        


