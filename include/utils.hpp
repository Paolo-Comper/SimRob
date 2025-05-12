#ifndef UTILS_H
#define UTILS_H

#define GLCall(x) \
    GLClearError();\
    x; \
    GLLogCall();\

#endif // UTILS_H
