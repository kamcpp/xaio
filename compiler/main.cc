/*  The MIT License (MIT)
 *
 *  Copyright (c) 2015 LabCrypto Org.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include <iostream>
#include <vector>
#include <string>

#include "compiler.h"
#include "xidl.h"

extern ::xaio::xaio_xidl *xidl;

void print_help_msg() {
  std::cout << std::endl;
  std::cout << "Xaio RPC" << std::endl;
  std::cout << "LabCrypto Org. 2015-2017 Copyright" << std::endl;
  std::cout << "Usage: xaio [OPTION]... [HOT-FILE]..." << std::endl;
  std::cout << "  OPTIONS:" << std::endl;
  std::cout << "    --java                     Generate java sources. [Default: MANDATORY]" << std::endl;
  std::cout << "    --cc                       Generate C++ sources. [Default: MANDATORY]" << std::endl;
  std::cout << "    --cmake                    Generate CMake file for C++ sources. [Default: disabled]" << std::endl;
  std::cout << "    --client                   Generate sources for C++ client. [Default: disabled]" << std::endl;
  std::cout << "    --stub                     Generate a C++ stub for service. [Default: disabled]" << std::endl;
  std::cout << "    --use-space-for-indent     Generator will produce indents using spaces. [Default: enabled]" << std::endl;
  std::cout << "    --use-tab-for-indent       Generator will produce indents using tabs. [Default: disabled]" << std::endl;
  std::cout << "    --num-spaces-for-indent    Number of spaces for producing a single indent. [Default: 2]" << std::endl;
  std::cout << "    --out                      Path to output directory. [Default: MANDATORY]" << std::endl;
  std::cout << "    --parse                    Displays parse result in a tree format. [Default: disabled]" << std::endl;
  std::cout << "    --dont-generate            Don't generate sources. [Default: disabled]" << std::endl;
  std::cout << "    --extra-module             A dot seperated string which will be added to module. [Default: '']" << std::endl;
  std::cout << std::endl;
  std::cout << "For more information and examples, please visit https://github.com/labcrypto/xaio" << std::endl;
  std::cout << std::endl;
  exit(1);
}

int main(int argc, char **argv) {
  bool is_cc = false;
  bool use_space_for_indent = true;
  bool gen_cmake = false;
  bool gen_client = false;
  bool gen_stub = false;
  bool dont_gen = false;
  bool parse = false;
  uint8_t num_spaces_for_indent = 2;
  char *out_dir = nullptr;
  char *extra_module = nullptr;
  ::std::vector<char *> xidls_to_process;
  for (uint16_t i = 1; i < argc;) {
    if (strcmp(argv[i], "--cc") == 0) {
      is_cc = true;
      i++;
    } else if (strcmp(argv[i], "--use-space-for-indent") == 0) {
      use_space_for_indent = true;
      i++;
    } else if (strcmp(argv[i], "--use-tab-for-indent") == 0) {
      use_space_for_indent = false;
      i++;
    } else if (strcmp(argv[i], "--camke") == 0) {
      gen_cmake = true;
      i++;
    } else if (strcmp(argv[i], "--client") == 0) {
      gen_client = true;
      i++;
    } else if (strcmp(argv[i], "--stub") == 0) {
      gen_stub = true;
      i++;
    } else if (strcmp(argv[i], "--dont-generate") == 0) {
      dont_gen = true;
      i++;
    } else if (strcmp(argv[i], "--parse") == 0) {
      parse = true;
      i++;
    } else if (strcmp(argv[i], "--help") == 0) {
      print_help_msg();
    } else if (strcmp(argv[i], "--num-spaces-for-indent") == 0) {
      num_spaces_for_indent = atoi(argv[i + 1]);
      i += 2;
    } else if (strcmp(argv[i], "--out") == 0) {
      out_dir = argv[i + 1];
      i += 2;
    } else if (strcmp(argv[i], "--extra-module") == 0) {
      extra_module = argv[i + 1];
      i += 2;
    } else {
      if (strlen(argv[i]) > 1 && strncmp(argv[i], "--", 2) == 0) {
        printf("ERROR: Unknown option: %s\n", argv[i]);
        print_help_msg();
      }
      xidls_to_process.push_back(argv[i]);
      i++;
    }
  }
  if (not is_cc) {
    ::std::cout << "ERROR: Target language is not specified. Use --cc for C++ generation." << ::std::endl;
    print_help_msg();
    exit(1);
  }
  if (not out_dir) {
    std::cout << "ERROR: Output director is not specified." << std::endl;
    print_help_msg();
    exit(1);
  }
  if (xidls_to_process.size() == 0) {
    std::cout << "ERROR: No xidl file is selected for compilation." << std::endl;
    print_help_msg();
    exit(1);
  }
  for (uint16_t i = 0; i < xidls_to_process.size(); i++) {
    yyin = fopen(xidls_to_process[i], "r+");
    if (!yyin) {
      printf("ERROR: File can't be opened.\n");
      exit(1);
    }
    yyparse();
    if (parse) {
      xidl->Display();
    }
    ::xaio::xaio_compiler_options compiler_options;
    compiler_options.out_dir = out_dir;
    compiler_options.use_space_for_indent = use_space_for_indent;
    compiler_options.num_spaces_for_indent = num_spaces_for_indent;
    compiler_options.gen_cmake = gen_cmake;
    compiler_options.gen_client = gen_client;
    compiler_options.gen_stub = gen_stub;
    compiler_options.extra_module = extra_module;
    ::xaio::xaio_compiler *compiler = nullptr;
    if (is_cc) {
      compiler = new ::xaio::xaio_compiler::xaio_cc_compiler;
      ::xaio::xaio_compiler::xaio_cc_compiler* cc_compiler =
          dynamic_cast<::xaio::xaio_compiler::xaio_cc_compiler *>(compiler);
      compiler->compiler(xidle, compiler_options);
      if (gen_cmake) {
        // ccGenerator->GenerateMakefile(currentHot, generationConfig);
      }
      if (gen_client) {
        // ccGenerator->GenerateClient(currentHot, generationConfig);
      }
      if (gen_stub) {
        // ccGenerator->GenerateStub(currentHot, generationConfig);
      }
    }
    delete compiler;
    delete xidl;
    xidl = nullptr;
  }
  return 0;
}
