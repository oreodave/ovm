/* Copyright (C) 2023 Aryadev Chavali

 * You may distribute and modify this code under the terms of the
 * GPLv2 license.  You should have received a copy of the GPLv2
 * license with this file.  If not, please write to:
 * aryadev@aryadevchavali.com.

 * Created: 2023-10-24
 * Author: Aryadev Chavali
 * Description: Parser for assembly language
 */

#ifndef PARSER_H
#define PARSER_H

#include "./lexer.h"

#include <lib/inst.h>

typedef enum
{
  PERR_OK = 0,
  PERR_INTEGER_OVERFLOW,
  PERR_NOT_A_NUMBER,
  PERR_EXPECTED_UTYPE,
  PERR_EXPECTED_TYPE,
  PERR_EXPECTED_SYMBOL,
  PERR_EXPECTED_OPERAND,
  PERR_UNKNOWN_OPERATOR,
  PERR_INVALID_RELATIVE_ADDRESS,
  PERR_UNKNOWN_LABEL,
} perr_t;

const char *perr_as_cstr(perr_t);

typedef struct
{
  inst_t instruction;
  char *label;
  s_word relative_address;
  enum PResult_Type
  {
    PRES_LABEL = 0,
    PRES_LABEL_ADDRESS,
    PRES_RELATIVE_ADDRESS,
    PRES_COMPLETE_RESULT,
  } type;
} presult_t;

perr_t parse_next(token_stream_t *, presult_t *);
perr_t process_presults(presult_t *, size_t, inst_t **, size_t *);
perr_t parse_stream(token_stream_t *, inst_t **, size_t *);

#endif
