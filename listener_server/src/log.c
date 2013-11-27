#include "log.h"
#include <stdarg.h>
#include <stdlib.h>

static FILE * log_file = NULL;

static void log_file_close() {
  fclose(log_file);
}

static const char *_error_level_names[] = {"INFO", "WARNING", "ERROR"};

int log_file_open(const char * filepath) {
  FILE * tmp = stderr;
  log_file = stderr;
  if (filepath && strcmp(filepath, "-") != 0) {
    tmp = fopen(filepath, "w");
    if (tmp) {
      log_file = tmp;
      atexit(log_file_close);
    }
    else {
      fprintf(stderr, "Unable to open log file %s because %s. Logging to stdout\n", filepath, strerror(errno));
      log_file = stderr;
    }
  }
  return 0;
}

int Log(enum error_level_t level, const char * msg, ...) {
  int err;
  va_list varargs;
  va_start(varargs, msg);

  err = fprintf(log_file, "%s: ", _error_level_names[level]);
  if (err < 0) {
    fprintf(stderr, "Error when logging. Error %d\n", ferror(log_file));
    goto log_exit;
  }
  err = vfprintf(log_file, msg, varargs);
  if (err < 0) {
    fprintf(stderr, "Error when logging message. Error %d\n", ferror(log_file));
    goto log_exit;
  }
  err = fprintf(log_file, "\n");
  if (err < 0) {
    fprintf(stderr, "Error when logging. Error %d\n", ferror(log_file));
    goto log_exit;
  }
  log_exit:
  return err;
}
