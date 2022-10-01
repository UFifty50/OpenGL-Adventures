#ifdef COMMON_HPP
#   error "common.hpp defined elsewhere"
#endif

#define USE_ASSIMP


#if defined(USE_ASSIMP) and defined(USE_OBJLOADER)
#   error Only one of USE_ASSIMP or USE_OBJ_LOADER can be defined
#elif !defined(USE_ASSIMP) and !defined(USE_OBJLOADER)
#   error "One of USE_ASSIMP or USE_OBJ_LOADER must be defined"
#endif
