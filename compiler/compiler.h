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

#pragma once

#include <stdlib.h>
#include <stdint.h>

#include <string>
#include <iostream>

namespace xaio {
namespace compiler {
  struct xaio_xidl;
  struct xaio_interface;
  struct xaio_struct;
  struct xaio_operation;
  struct xaio_module;
  struct xaio_arg;
  struct xaio_decl;
  struct xaio_compiler_options {
    bool gen_cmake;
    bool gen_client;
    bool gen_stub;
    bool use_space_for_indent;
    uint8_t num_spaces_for_indent;
    ::std::string out_dir;
    ::std::string extra_module;
    ::std::string get_indent_str() {
      if (use_space_for_indent) {
        ::std::string indent_str = "";
        for (uint8_t i = 0; i < num_space_for_indent; i++) {
          indent_str += " ";
        }
        return indent_str;
      }
      return "\t";
    }  
  };
  class xaio_compiler {
  public:
    xaio_compiler() = default;
    virtual ~xaio_compiler() = default;
  public:
    virtual void compile(xaio_xidl &, xaio_compiler_options &) = 0;
  };
}
}
