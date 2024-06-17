#include "grep.h"

int main(int argc, char **argv)

{
  if ((pars_command_line(argc, argv, SHORT_OPTIONS, &options)) == 0) {
    options.regex_arguments_length = 0;

    parse_files(argc - optind, argv + optind, &options);

  } else
    printf("Some troubles were happened");
  return 0;
}

int pars_command_line(int argc, char *const *argv, const char *SHORT_OPTIONS,
                      struct OptionObject *options) {
  char current_argument;
  int flag_succees = 0;
  while ((current_argument = getopt(argc, argv, SHORT_OPTIONS)) != -1) {
    switch (current_argument) {
      case 'e':
        options->e = 1;
        parse_argument_regex(options, optarg);
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'f':
        options->f = 1;
        parse_argument_files(options, optarg);
        break;
      case 'o':
        options->o = 1;
        break;
      default:
        printf("Try 's21_grep --help' for more information.\n");
        flag_succees = 1;
        break;
    }
  }
  if (options->e == 0 && options->f == 0)
    parse_argument_regex(options, argv[optind++]);
  return flag_succees;
}

void print_files_names_and_lines_count(char *file_names, int line_count,
                                       struct OptionObject *options,
                                       int files_args) {
  if (options->h == 0 && options->l == 0 && files_args > 1)
    printf("%s:", file_names);
  if (options->n == 1 && options->l == 0) printf("%d:", line_count);
}

void print_lines_count(int line_count, struct OptionObject *options) {
  if (options->n == 1 && options->l == 0) printf("%d: ", line_count);
}

void parse_one_file(int files_count, FILE *filename, char *const files[],
                    struct OptionObject *options) {
  regex_t regex;
  char *line = NULL;
  ssize_t nread;
  size_t len = 0;
  int line_counter = 0, lines_count = 0, value_result = 0;
  while ((nread = getline(&line, &len, filename)) != -1) {
    line_counter++;
    value_result = options->i == 1
                       ? regcomp(&regex, options->regex_arguments, REG_ICASE)
                       : regcomp(&regex, options->regex_arguments, 0);
    value_result = regexec(&regex, line, 0, NULL, 0);
    if ((value_result == 0 && options->v == 0) ||
        (value_result == 1 && options->v == 1)) {
      if (options->c == 0 && options->l == 0) {
        if (options->h == 0 && options->o == 0 && files_count > 1)
          printf("%s:", *files);
        if (options->n == 1 && options->l == 0 && options->o == 0)
          printf("%d:", line_counter);
        if (options->o == 1) {
          option_flag_o(files_count, &regex, line, files, options,
                        line_counter);
        } else
          printf("%s", line);
      }
      lines_count++;
    }
    regfree(&regex);
  }

  if (options->c == 1 && options->l == 0) {
    if (options->h == 0)
      if (files_count > 1 && lines_count > 0) printf("%s:", *files);
    printf("%d\n", lines_count);
  }
  if (options->c == 1 && options->l == 1) {
    if (options->h == 0 && lines_count > 0) {
      printf("%s\n", *files);
    } else if (lines_count > 0)
      printf("%s\n", *files);
  }
  if (options->l == 1 && files_count > 0 && options->c == 0 && lines_count > 0)
    printf("%s\n", *files);

  free(line);

  // regfree(&regex);
}

void parse_argument_regex(struct OptionObject *options, char *optarg) {
  if (options->regex_arguments_length != 0) {
    strcat(options->regex_arguments + options->regex_arguments_length, "|");
    options->regex_arguments_length++;
  }
  options->regex_arguments_length += sprintf(
      options->regex_arguments + options->regex_arguments_length, "%s", optarg);
}

void parse_argument_files(struct OptionObject *options, char *optarg) {
  FILE *f = fopen(optarg, "r");
  if (f == NULL)
    show_error_message(optarg);

  else {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    while ((nread = getline(&line, &len, f)) != -1) {
      if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
      parse_argument_regex(options, line);
    }
    free(line);
    fclose(f);
  }
}

void option_flag_o(int files_count, regex_t *regex, char *line,
                   char *const *filename, struct OptionObject *options,
                   int line_number) {
  regmatch_t match;
  int offset = 0;
  while (1 == 1) {
    int res = regexec(regex, line + offset, 1, &match, 0);
    if (res != 0) {
      break;
    }
    if (!options->h && ((files_count - optind) > 1)) {
      printf("%s:", *filename);
    }
    if (options->n) {
      printf("%d:", line_number);
    }
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[offset + i]);
    }
    puts("");
    offset += match.rm_eo;
  }
  regfree(regex);
}

void parse_files(int files_count, char *const files[],
                 struct OptionObject *options) {
  int files_args = files_count;
  FILE *stream;
  while (files_count > 0) {
    stream = fopen(*files, "r");
    if (stream != NULL) {
      parse_one_file(files_args, stream, files, options);
      fclose(stream);
    } else if (options->s == 0) {
      show_error_message(*files);
    }
    files++;
    files_count--;
  }
}

void show_error_message(const char *const file) {
  if (options.s == 0) printf("s21grep: %s No such file or directory\n", file);
}