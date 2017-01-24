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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include <string>
#include <fstream>
#include <streambuf>
#include <stdexcept>
#include <iostream>

#include "string_helper.h"
#include "fs.h"

namespace xaio {
namespace compiler {

  void xaio_fs::make_dir(::std::string path_str) {
    ::std::string tr_path = xaio_string_helper::trim(path_str);
    ::std::string usable_path = StringHelper::Replace(tr_path, "\\", "/");
    ::std::size_t index = usable_path.find("/");
    while (index != ::std::string::npos) {
      if (index > 0) {
        ::std::string path = usable_path.substr(0, index);
        struct stat st = {0};
        if (stat(path.c_str(), &st) == -1) {
          int res = mkdir(path.c_str(), 0700);
          if (res) {
            ::std::cout << "ERROR: Error while making dir: '" << 
              path << "', errno: " << errno << ::std::endl;
          }
        }
      }
      index = usable_path.find("/", index + 1);
    }
    const char *path = usable_path.c_str();
    struct stat st = {0};
    if (stat(path, &st) == -1) {
      mkdir(path, 0700);
    }
  }

  void xaio_fs::read_file(std::string path, std::string &content) {
    ::std::ifstream f(path.c_str());
    if (f.fail()) {
      throw ::std::runtime_error("File couldn't be opened.");
    }
    f.seekg(0, ::std::ios::end);   
    content.reserve(t.tellg());
    f.seekg(0, ::std::ios::beg);
    content.assign (
      (::std::istreambuf_iterator<char>(t)),
      ::std::istreambuf_iterator<char>()
    );
  }
    
  void xaio_fs::write_file(std::string path , std::string &content) {
    ::std::ofstream f;
    f.open(path.c_str() , ::std::ios::trunc);
    if (f.fail()) {
      throw ::std::runtime_error("File couldn't be opened.");
    }
    f.write(content.c_str() , content.size());
    f.close();
  }
}
}
