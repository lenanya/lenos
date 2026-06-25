#include "serial.h"
#include "string.h"
#include "stdarg.h"

// write a null terminated string over serial
void serial_print(char* s) {
  u32 len = strlen(s);
  for (u32 i = 0; i < len; ++i) {
    serial_putc(s[i]);
  }
}

// write a null terminated string over serial and append a newline
void serial_println(char* s) {
  serial_print(s);
  serial_putc('\n');
}

#define ereturn do {va_end(args);return;} while (0)

// write a formatted string over serial
// TODO: refactor and clean up
void serial_printf(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  u32 l = strlen(fmt);
  u32 i = 0;
  while (i < l) {
    char c = fmt[i];
    if (c == '%') {
      ++i;
      if (i >= l) ereturn;
      c = fmt[i];
      u32 zeropadding = 0;
      u32 leftpadding = 0;
      u32 rightpadding = 0;
      if (c == '*') {
        leftpadding = va_arg(args, u32);
      } else if (c == '-') {
        ++i;
        if (i >= l) ereturn;
        c = fmt[i];
        if (c == '*') {
          rightpadding = va_arg(args, u32);
        } else {
          char padding_amount_str[16];
          u32 pad_index = 0;
          while (is_digit(c)) {
            padding_amount_str[pad_index++] = c;
            ++i;
            if (i >= l) ereturn;
            c = fmt[i];
          }
          if (pad_index > 0) {
            padding_amount_str[pad_index] = 0;
            rightpadding = stou32(padding_amount_str);
          }
        }
      } else if (c == '0') {
        ++i;
        if (i >= l) ereturn;
        c = fmt[i];
        char padding_amount_str[16];
        u32 pad_index = 0;
        while (is_digit(c)) {
          padding_amount_str[pad_index++] = c;
          ++i;
          if (i >= l) ereturn;
          c = fmt[i];
        }
        if (pad_index > 0) {
          padding_amount_str[pad_index] = 0;
          zeropadding = stou32(padding_amount_str);
        }
      } else if (is_digit(c)) {
        char padding_amount_str[16];
        u32 pad_index = 0;
        while (is_digit(c)) {
          padding_amount_str[pad_index++] = c;
          ++i;
          if (i >= l) ereturn;
          c = fmt[i];
        }
        if (pad_index > 0) {
          padding_amount_str[pad_index] = 0;
          leftpadding = stou32(padding_amount_str);
        }
      }
      char temp[32];
      switch (c) {
        case 'd': {
          i32 arg = va_arg(args, i32);
          itoa_buffer(arg, temp);
          u32 l = strlen(temp);
          while (leftpadding-- > l) serial_putc(' ');
          while (zeropadding-- > l) serial_putc('0');
          serial_print(temp);
          while (rightpadding-- > l) serial_putc(' ');
          break;
        }
        case 'u': {
          u32 arg = va_arg(args, u32);
          utoa_buffer(arg, temp);
          u32 l = strlen(temp);
          while (leftpadding-- > l) serial_putc(' ');
          while (zeropadding-- > l) serial_putc('0');
          serial_print(temp);
          while (rightpadding-- > l) serial_putc(' ');
          break;
        }
        case 's': {
          char* arg = va_arg(args, char*);
          u32 l = strlen(arg);
          while (leftpadding-- > l) serial_putc(' ');
          serial_print(arg);
          while (rightpadding-- > l) serial_putc(' ');
          break;
        }
        case 'c': {
          i32 arg = va_arg(args, i32);
          while (leftpadding-- > 1) serial_putc(' ');
          serial_putc(arg);
          while (rightpadding-- > 1) serial_putc(' ');
          break;
        }
        case 'b': {
          i32 arg = va_arg(args, i32);
          char* str = arg ? "true" : "false";
          u32 l = strlen(str);
          while (leftpadding-- > l) serial_putc(' ');
          serial_print(str);
          while (rightpadding-- > l) serial_putc(' ');
          break;
        }
        case 'x': {
          void* arg = va_arg(args, void*);
          u32_to_hex_buffer((u32)arg, temp, false);
          u32 l = strlen(temp);
          while (leftpadding-- > l) serial_putc(' ');
          while (zeropadding-- > l) serial_putc('0');
          serial_print(temp);
          while (rightpadding-- > l) serial_putc(' ');
          break;
        }
        case 'X': {
          void* arg = va_arg(args, void*);
          u32_to_hex_buffer((u32)arg, temp, true);
          u32 l = strlen(temp);
          while (leftpadding-- > l) serial_putc(' ');
          while (zeropadding-- > l) serial_putc('0');
          serial_print(temp);
          while (rightpadding-- > l) serial_putc(' ');
          break;
        }
        default: {
          va_end(args);
          return;
        }
      }
    } else {
      serial_putc(c);
    }
    ++i;
  }
  va_end(args);
}