//
// Created by alan on 18-6-21.
//

#ifndef HIMPP_VDEC_DEMO_LOG_H
#define HIMPP_VDEC_DEMO_LOG_H

//
// Created by alan on 18-4-9.
//

#ifdef DEBUG
#define log_debug_str(s)    do{\
                        printf("function: %s, line: %d, msg: %s\n",\
                                __FUNCTION__, __LINE__, s); \
                    }while(0)

#define log_debug_str_str(sa, sb)    do{\
                        printf("function: %s, line: %d, msg: %s : %s\n",\
                                __FUNCTION__, __LINE__, sa, sb); \
                    }while(0)

#define log_debug_int(s, i)    do{\
                        printf("function: %s, line: %d, msg: %s %ld\n",\
                                __FUNCTION__, __LINE__, s, (long int)i); \
                    }while(0)

#define log_debug_ul(s, ul)    do{\
                        printf("function: %s, line: %d, msg: %s%lu\n",\
                                __FUNCTION__, __LINE__, s, (unsigned long)ul); \
                    }while(0)
#else


#define log_debug_int(s, i)
#define log_debug_ul(s, ul)
#define log_debug_str(s)
#define log_debug_str_str(sa, sb)

#endif

#ifdef INFO
#define log_info_str(s)    do{\
                        printf("function: %s, line: %d, msg: %s\n",\
                                __FUNCTION__, __LINE__, s); \
                    }while(0)

#define log_info_str_str(sa, sb)    do{\
                        printf("function: %s, line: %d, msg: %s : %s\n",\
                                __FUNCTION__, __LINE__, sa, sb); \
                    }while(0)

#define log_info_int(s, i)    do{\
                        printf("function: %s, line: %d, msg: %s %ld\n",\
                                __FUNCTION__, __LINE__, s, (long int)i); \
                    }while(0)

#define log_info_ul(s, ul)    do{\
                        printf("[%s], [%d]: %s%lu\n",\
                                 __FUNCTION__, __LINE__, s, (unsigned long)ul); \
                    }while(0)

#else

#define log_info_int(s, i)
#define log_info_ul(s, ul)
#define log_info_str(s)
#define log_info_str_str(sa, sb)

#endif

#define log_handle_error(s)   do{\
                                    printf("[ERROR] function: [%s], line: [%d], msg: %s\n",\
                                    __FUNCTION__, __LINE__, s); \
                                }while(0)



#endif //HIMPP_VDEC_DEMO_LOG_H
