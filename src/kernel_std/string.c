#include "string.h"
#include "../common.h"
#include "mem.h"
#include "da.h"
#include "io.h"

u32 strlen(char* s) {
  u32 i = 0;
  while (s[i] != 0) ++i;
  return i;
}

bool strcmp(char* a, char* b) {
  u32 la, lb;
  la = strlen(a);
  lb = strlen(b);
  if (la != lb) return false;
  for (u32 i = 0; i < la; ++i) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

bool strncmp(char* a, char* b, u32 n) {
  for (u32 i = 0; i < n; ++i) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

char* strdup(char* s) {
  u32 l = strlen(s);
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

char* utoa(u32 n) {
  String_Buffer sb = {0};
  String_Buffer res = {0};
  while (n > 0) {
    u8 c = (n%10) + '0';
    da_append(&sb, c);
    n = n / 10;
  }
  for (u32 i = 1; i <= sb.size ; ++i) {
    if (res.size < 1 && sb.items[sb.size - i] == '0') continue;
    da_append(&res, sb.items[sb.size - i]);
  }
  if (sb.size == 0) da_append(&res, '0');
  da_append(&res, 0);
  free(sb.items);
  return res.items;
}

char* itoa(i32 n) {
  String_Buffer sb = {0};
  String_Buffer res = {0};
  if (n < 0) {
    da_append(&res, '-');
    n *= -1;
  }
  while (n > 0) {
    u8 c = (n%10) + '0';
    da_append(&sb, c);
    n = n / 10;
  }
  for (i32 i = (i32)sb.size - 1; i >= 0; --i) {
    if (res.size < 1 && sb.items[i] == '0') continue;
    da_append(&res, sb.items[i]);
  }
  if (sb.size == 0) da_append(&res, '0');
  da_append(&res, 0);
  free(sb.items);
  return res.items;
}

u32 stou32(char* s) {
  u32 l = strlen(s); 
  u32 mult = 1;
  u32 res = 0;
  for (i32 i = l - 1; i >= 0; --i) {
    res += (mult * (s[i] - '0'));
    mult *= 10;
  }
  return res;
}

bool is_digit(char c) {
  if (c >= '0' && c <= '9') return true;
  return false;
}

bool s_is_digits(char* s) {
  for (u32 i = 0; i < strlen(s); ++i) {
    if (!is_digit(s[i])) return false;
  }
  return true;
}