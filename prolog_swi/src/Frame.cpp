/******************************************************************************
 * Copyright (C) 2016 by Ralf Kaestner                                        *
 * ralf.kaestner@gmail.com                                                    *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the Lesser GNU General Public License as published by*
 * the Free Software Foundation; either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               *
 * Lesser GNU General Public License for more details.                        *
 *                                                                            *
 * You should have received a copy of the Lesser GNU General Public License   *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.       *
 ******************************************************************************/

#include <SWI-Prolog.h>

#include <prolog_swi/Context.h>

#include "prolog_swi/Frame.h"

namespace prolog { namespace swi {

/*****************************************************************************/
/* Constructors and Destructor                                               */
/*****************************************************************************/

Frame::Frame() :
  impl_(new Impl()) {
}

Frame::Frame(const Frame& src) :
  impl_(src.impl_) {
}

Frame::~Frame() {
}

Frame::Impl::Impl() :
  handle_(0) {
}

Frame::Impl::~Impl() {
  close();
}

/*****************************************************************************/
/* Accessors                                                                 */
/*****************************************************************************/

bool Frame::isOpen() const {
  if (impl_.get())
    return impl_->handle_;
  else
    return false;
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

bool Frame::open() {
  if (impl_.get())
    return impl_->open();
  else
    return false;
}

void Frame::rewind() {
  if (impl_.get())
    impl_->rewind();
}

void Frame::close() {
  if (impl_.get())
    impl_->close();
}

bool Frame::Impl::open() {
  if (!handle_)
    handle_ = PL_open_foreign_frame();
  
  return handle_;
}

void Frame::Impl::rewind() {
  if (handle_)
    PL_rewind_foreign_frame(handle_);
}

void Frame::Impl::close() {
  if (handle_) {
    PL_close_foreign_frame(handle_);
    
    handle_ = 0;
  }
}

}}
