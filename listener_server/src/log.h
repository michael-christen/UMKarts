#ifndef __LOG_H_
#define __LOG_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>

enum error_level_t {
  EINFO,
  EWARNING,
  EERROR,
  ENUMLEVELS,
};

int log_file_open(const char * filepath); 

int Log(enum error_level_t level, const char * msg, ...);



#endif /* __LOG_H_ */
