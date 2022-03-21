#ifndef COLOR_H

#ifdef USE_COLOR
    #define NONE_COLOR "\033[0m"
    #define BLACK "\033[0;30m"
    #define DARK_GRAY "\033[1;30m"
    #define RED "\033[0;31m"
    #define LIGHT_RED "\033[1;31m"
    #define GREEN "\033[0;32m"
    #define LIGHT_GREEN "\033[1;32m"
    #define ORANGE "\033[0;33m"
    #define YELLOW "\033[1;33m"
    #define BLUE "\033[0;34m"
    #define LIGHT_BLUE "\033[1;34m"
    #define PURPLE "\033[0;35m"
    #define LIGHT_PURPLE "\033[1;35m"
    #define CYAN "\033[0;36m"
    #define LIGHT_CYAN "\033[1;36m"
    #define LIGHT_GRAY "\033[0;37m"
    #define WHITE "\033[1;37m"
#else 
    #define NONE_COLOR ""
    #define BLACK ""
    #define DARK_GRAY ""
    #define RED ""
    #define LIGHT_RED ""
    #define GREEN ""
    #define LIGHT_GREEN ""
    #define ORANGE ""
    #define YELLOW ""
    #define BLUE ""
    #define LIGHT_BLUE ""
    #define PURPLE ""
    #define LIGHT_PURPLE ""
    #define CYAN ""
    #define LIGHT_CYAN ""
    #define LIGHT_GRAY ""
    #define WHITE ""
#endif

#define SCOLOR(color, string) (color) + (string) + NONE_COLOR
#define CCOLOR(color, c_string) color c_string NONE_COLOR



#define UNUSED(a) ((void)(a))

#define ERR_MSG_V(msg, ...) \
    printf(LIGHT_RED"** ERROR: " msg NONE_COLOR"\n", ##__VA_ARGS__)

#define INFO_MSG_V(msg, ...) \
    printf(LIGHT_GREEN"** INFO: " msg NONE_COLOR"\n", ##__VA_ARGS__)

#define WARN_MSG_V(msg, ...) \
    printf(YELLOW"** WARN: " msg NONE_COLOR"\n", ##__VA_ARGS__)




#define COLOR_H
#endif
