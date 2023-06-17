// #define DEBUG
#ifdef DEBUG
#define PrintDebug(...) Serial.print(__VA_ARGS__)
#define PrintDebugLn(...) Serial.println(__VA_ARGS__)
#else
#define PrintDebug(...)
#define PrintDebugLn(...)
#endif

#define INFO
#ifdef INFO
#define PrintInfo(...) Serial.print(__VA_ARGS__)
#define PrintInfoLn(...) Serial.println(__VA_ARGS__)
#else
#define PrintInfo(...)
#define PrintInfoLn(...)
#endif