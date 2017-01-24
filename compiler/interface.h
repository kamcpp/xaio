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
#include <stdio.h>
#include <stdint.h>

#include <vector>
#include <string>
#include <iostream>

namespace xaio {
namespace compiler {
  struct xaio_module;
  struct xaio_operation;
  struct xaio_interface {
    enum {
      STATELESS,
      STATEFUL
    } interface_type;
    ::std::string name;
    ::std::vector<xaio_operation *> operations;
    xaio_module *module;
    xaio_interface (
      std::string interface_type_str,
      std::string name,
      xaio_module *module
    ) : name(name),
        module(module) {
      set_interface_type(interface_type_str);
    }

    void set_interface_type(::std::string interface_type_str) {
      if (interface_type_str == "stateless") {
        service_type = STATELESS;
      } else if (interface_type_str == "stateful") {
        service_type = STATEFUL;
      } else {
        ::std::cout << "SEMANTIC ERROR: Wrong type for service. " <<
          "You should either use 'stateless' or 'stateful'." << std::endl;
        exit(1);
      }
    }
    virtual ::std::string get_fq_name() const;
    virtual uint32_t get_hash() const;
  };
}
}
