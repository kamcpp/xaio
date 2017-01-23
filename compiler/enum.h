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

#include <map>

#include "declaration.h"

namespace xaio {
namespace compiler {
  struct xaio_module;
  struct xaio_enum {
    ::std::string name;
    ::std::map<::std::string, uint16_t> items;
    ::std::map<uint16_t, ::std::string> rev_items;
    xaio_module *module;
    xaio_enum (
      xaio_module *module
    ) : module(module) {
    }
  public:
    void add_item(::std::string name, uint16_t value) {
      items[name] = value;
      rev_items[value] = name;
    }
    ::std::string get_item_name(uint16_t item_value) {
      return rev_items[itemValue];
    }
    uint16_t get_item_value(::std::string item_name) {
      return items[item_name];
    }
  };
}
}