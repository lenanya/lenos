#include "string.h"
#include "../common.h"
#include "mem.h"
#include "da.h"
#include "io.h"

int strlen(char* s) {
  int i = 0;
  while (s[i] != 0) ++i;
  return i;
}

bool strcmp(char* a, char* b) {
  int la, lb;
  la = strlen(a);
  lb = strlen(b);
  if (la != lb) return false;
  for (int i = 0; i < la; ++i) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

bool strncmp(char* a, char* b, u32 n) {
  for (int i = 0; i < n; ++i) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

char* strdup(char* s) {
  int l = strlen(s);
  char* ret = (char*)malloc(l+1);
  memncpy(s, ret, l+1);
  return ret;
}

char HEX_CHARS[16] = "0123456789abcdef";

char* u32_to_hex(u32 n) {
  String_Buffer sb = {0};
  String_Buffer ret = {0};
  da_append(&ret, '0');
  da_append(&ret, 'x');
  if (n == 0) {
    da_append(&ret, '0');
  } else {
    while (n != 0) {
      u32 rem = n % 16;
      da_append(&sb, HEX_CHARS[rem]);
      n = (u32)(n / 16);
    }
    for (u32 i = 1; i <= sb.size; ++i) {
      da_append(&ret, sb.items[sb.size - i])
    }
  }
  da_append(&ret, 0);
  free(sb.items);
  return ret.items;
}

char* utoa(u32 i) {
  String_Buffer sb = {0};
  String_Buffer res = {0};
  while (i > 0) {
    u8 c = (i%10) + '0';
    da_append(&sb, c);
    i = i / 10;
  }
  for (i32 i = 1; i <= sb.size ; ++i) {
    if (res.size < 1 && sb.items[sb.size - i] == '0') continue;
    da_append(&res, sb.items[sb.size - i]);
  }
  if (sb.size == 0) da_append(&res, '0');
  da_append(&res, 0);
  free(sb.items);
  return res.items;
}

char* itoa(i32 i) {
  String_Buffer sb = {0};
  String_Buffer res = {0};
  if (i < 0) {
    da_append(&res, '-');
    i *= -1;
  }
  while (i > 0) {
    u8 c = (i%10) + '0';
    da_append(&sb, c);
    i = i / 10;
  }
  for (i32 i = sb.size - 1; i >= 0; --i) {
    if (res.size < 1 && sb.items[i] == '0') continue;
    da_append(&res, sb.items[i]);
  }
  if (sb.size == 0) da_append(&res, '0');
  da_append(&res, 0);
  free(sb.items);
  return res.items;
}