#ifndef LOG_H_
#define LOG_H_

#define TRACE_FUNCTION_NAME printf("%s() :: ", __func__);
#define TRACE_POSITION                                                      \
  do {                                                                      \
    fprintf(stderr, "%s:%s():%d :: ERROR: ", __FILE__, __func__, __LINE__); \
  } while(0);                                                               \

#endif // LOG_H_
