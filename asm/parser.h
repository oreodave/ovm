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
  PERR_EXPECTED_LABEL,
  PERR_EXPECTED_OPERAND,
  PERR_INVALID_RELATIVE_ADDRESS,
  PERR_UNKNOWN_OPERATOR,
  PERR_UNKNOWN_LABEL,
} perr_t;

const char *perr_as_cstr(perr_t);

// TODO: Refactor parser.c
typedef struct
{
  inst_t instruction;
  darr_t instructions;
  struct PLabel
  {
    char *name;
    size_t size;
  } label;
  s_word address;
  enum PResult_Type
  {
    PRES_LABEL = 0,
    PRES_LABEL_ADDRESS,
    PRES_GLOBAL_LABEL,
    PRES_RELATIVE_ADDRESS,
    PRES_PP_CONST,
    PRES_COMPLETE_RESULT,
  } type;
} presult_t;

// TODO: Implement these
presult_t presult_label(const char *, size_t, s_word);
presult_t presult_instruction(inst_t);
presult_t presult_instructions(size_t);
presult_t presult_addr(s_word);
presult_t pres_pp_const(const char *, s_word, size_t);
// TODO: Refactor parser.c to use this instead
void presult_free(presult_t);

typedef struct
{
  char *name;
  size_t name_size;
  word addr;
} label_t;

label_t search_labels(label_t *, size_t, char *, size_t);

perr_t parse_next(token_stream_t *, presult_t *);
perr_t preprocessor(presult_t *, size_t, presult_t *);
perr_t process_presults(presult_t *, size_t, prog_t **);
perr_t parse_stream(token_stream_t *, prog_t **);

#endif
