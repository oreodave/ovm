/* Copyright (C) 2023 Aryadev Chavali

 * You may distribute and modify this code under the terms of the
 * GPLv2 license.  You should have received a copy of the GPLv2
 * license with this file.  If not, please write to:
 * aryadev@aryadevchavali.com.

 * Created: 2023-10-23
 * Author: Aryadev Chavali
 * Description: Assembly source code compiler, targeting OVM
 */

#include <lib/darr.h>

#include "./lexer.h"
#include "./parser.h"

void usage(const char *program_name, FILE *fp)
{
  fprintf(fp,
          "Usage: %s FILE OUT-FILE\n"
          "\tFILE: Source code to compile\n"
          "\tOUT-FILE: Name of file to store bytecode\n",
          program_name);
}

int main(int argc, char *argv[])
{
  int ret           = 0;
  char *source_file = "";
  char *out_file    = "";
  if (argc < 3)
  {
    usage(argv[0], stderr);
    return 1;
  }

  source_file   = argv[1];
  out_file      = argv[2];
  FILE *fp      = fopen(source_file, "rb");
  darr_t buffer = darr_read_file(fp);
  fclose(fp);

  token_stream_t tokens = tokenise_buffer(&buffer);
#if VERBOSE >= 1
  printf("[%sTOKENISER%s]: %lu bytes -> %lu tokens\n", TERM_GREEN, TERM_RESET,
         buffer.used, tokens.available);
#endif
  free(buffer.data);

  size_t number        = 0;
  inst_t *instructions = NULL;
  perr_t parse_error   = parse_stream(&tokens, &instructions, &number);
  if (parse_error)
  {
    size_t column = 0;
    size_t line   = 0;
    if (tokens.used < tokens.available)
    {
      token_t t = TOKEN_STREAM_AT(tokens.data, tokens.used);
      column    = t.column;
      line      = t.line;
    }
    fprintf(stderr, "%s:%lu:%lu: %s\n", source_file, line, column,
            perr_as_cstr(parse_error));
    ret = 255 - parse_error;
    goto end;
  }
#if VERBOSE >= 1
  printf("[%sPARSER%s]: %lu tokens -> %lu instructions\n", TERM_GREEN,
         TERM_RESET, tokens.available, number);
#endif

  fp = fopen(out_file, "wb");
  insts_write_bytecode_file(instructions, number, fp);
  fclose(fp);
#if VERBOSE >= 1
  printf("[%sCOMPILER%s]: Wrote bytecode to `%s`\n", TERM_GREEN, TERM_RESET,
         out_file);
#endif
end:
  // Free the tokens and parsed data
  if (tokens.data)
  {
    for (size_t i = 0; i < tokens.available; ++i)
      free(TOKEN_STREAM_AT(tokens.data, i).str);
    free(tokens.data);
  }
  if (instructions)
    free(instructions);
  return ret;
}
