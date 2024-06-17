#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef S21_GREP_IMPL
#define S21_GREP_IMPL 1

struct OptionObject {
  int e;
  int f;
  int i;
  int s;
  int v;
  int n;
  int h;
  int o;
  int l;
  int c;
  char *file_arg;
  char *regex_arg;
  char *query;
  char regex_arguments[1000];
  int regex_arguments_length;
};

const char SHORT_OPTIONS[] = "e:ivc::lnhsf:o";

struct OptionObject options = {0};

void option_flag_o(int argc, regex_t *re, char *line, char *const *filename,
                   struct OptionObject *options, int line_number);

int pars_command_line(int argc, char *const *argv, const char *SHORT_OPTIONS,

                      struct OptionObject *options);
void parse_files(int files_count, char *const files[],
                 struct OptionObject *options);

void show_error_message(const char *const file);

void parse_one_file(int files_count, FILE *filename, char *const files[],
                    struct OptionObject *options);

void pasrs_regex_files(struct OptionObject *options, char *regex_files);

void parse_argument_regex(struct OptionObject *options, char *optarg);

void parse_argument_files(struct OptionObject *options, char *optarg);

#endif