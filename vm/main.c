/* Copyright (C) 2023 Aryadev Chavali

 * You may distribute and modify this code under the terms of the
 * GPLv2 license.  You should have received a copy of the GPLv2
 * license with this file.  If not, please write to:
 * aryadev@aryadevchavali.com.

 * Created: 2023-10-15
 * Author: Aryadev Chavali
 * Description: Entrypoint to program
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "./inst.h"
#include "./runtime.h"

int interpret_bytecode(const char *filepath)
{
  FILE *fp             = fopen(filepath, "rb");
  size_t number        = 0;
  inst_t *instructions = insts_read_bytecode_file(fp, &number);
  fclose(fp);

  byte stack[256];
  vm_t vm = {0};
  vm_load_stack(&vm, stack, ARR_SIZE(stack));
  vm_load_program(&vm, instructions, number);
  err_t err = vm_execute_all(&vm);

  int ret = 0;
  if (err)
  {
    const char *error_str = err_as_cstr(err);
    fprintf(stderr, "[ERROR]: %s\n", error_str);
    vm_print_all(&vm, stderr);
    ret = 255 - err;
  }
  free(instructions);
  return ret;
}

int assemble_instructions(inst_t *instructions, size_t number,
                          const char *filepath)
{
  FILE *fp = fopen(filepath, "wb");
  insts_write_bytecode_file(instructions, number, fp);
  fclose(fp);
  return 0;
}

void usage(const char *program_name, FILE *out)
{
  fprintf(out,
          "Usage: %s [OPTIONS] FILE\n"
          "\t FILE: Bytecode file to execute\n"
          "\tOptions:\n"
          "\t\t To be developed...\n",
          program_name);
}

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    usage(argv[0], stderr);
    return 1;
  }
  const char *filename = argv[1];

  return interpret_bytecode(filename);
}
