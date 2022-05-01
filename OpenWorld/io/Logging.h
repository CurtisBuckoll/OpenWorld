#pragma once

#include <stdio.h>
#include <time.h>

static const char* kFmt = "%s%s%s\n";
static const char* kFmtTime = "%m-%d-%H:%M:%S\t";

// max message size is 512 (for now). At some point we may need to allow
// for async logging so this will have to be updated.
static constexpr uint64_t kBufferSize = 512;

enum LOGLEVEL {
  INFO = 0,
  WARN,
  ERRO,
};

static const char* LOGLEVELSTR[] = {
  "[INFO]\t",
  "[WARN]\t",
  "[ERROR]\t",
};

static void time_stamp( char* const buffer ) {
  time_t now = time(0);
  tm timeinfo;
  localtime_s(&timeinfo, &now);
  strftime(buffer, kBufferSize, kFmtTime, &timeinfo);
}

#define OW_LOG( level, log_str, ... ) do { \
  char msg[kBufferSize]; \
  time_stamp(msg); \
  sprintf_s(msg, kBufferSize, kFmt, msg, LOGLEVELSTR[level], log_str); \
  printf(msg, ##__VA_ARGS__ ); \
} \
while(0) \
