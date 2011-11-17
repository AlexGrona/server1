/*
 * Copyright (c) 2009, Xiliu Tang (xiliu.tang@gmail.com)
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 * 
 *     * Redistributions of source code must retain the above copyright 
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above 
 *       copyright notice, this list of conditions and the following 
 *       disclaimer in the documentation and/or other materials provided 
 *       with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Project Website http://code.google.com/p/server1/
 */
#ifndef CHECKBOOK_HPP_
#define CHECKBOOK_HPP_
#include "base/base.hpp"
#include "services/file_transfer/checkbook.pb.h"
class CheckBook : public FileTransfer::CheckBook {
 public:
  // New a checkbook or load the existing checkbook.
  static CheckBook *Create(
      const string &host, const string &port, const string &src_filename, const string &dest_filename);
  static CheckBook *Load(const string &checkbook_filename);
  bool Save(const string &filename) {
    Save(this, filename);
  }
  static bool Save(const FileTransfer::CheckBook *checkbook, const string &filename);
  string GetCheckBookDestFileName() const {
    return GetCheckBookDestFileName(&meta());
  }
  static string GetCheckBookDestFileName(
      const FileTransfer::MetaData *meta);
  string GetCheckBookSrcFileName() const;
  static int GetSliceSize() {
    return kSliceSize;
  }
  // The percent * 1000, 1000 means transfer finished.
  int Percent() const;
 private:
  static const int kSliceSize = 640 * 1024;
  static string InternalGetCheckBookSrcFileName(
      const string &host, const string &port,
      const string &src_filename,
      const string &dest_filename);

  static string GetEvpName(const string &host, const string &port,
                           const string &src_filename, const string &dest_filename);
  CheckBook() : FileTransfer::CheckBook() {
  }
};
#endif  // CHECKBOOK_HPP_
