/*
    This file is part of url_reader, utility to download some url content
    Copyright (C) 2012  Julien Thevenon ( julien_thevenon at yahoo.fr )

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifndef _DOWNLOAD_BUFFER_H_
#define _DOWNLOAD_BUFFER_H_

#include <stdio.h>

namespace quicky_url_reader
{
  /**
     Extensible buffer
  **/
  class download_buffer
  {
  public:
    download_buffer(void);
    void add_data(size_t p_size, void * p_data);
    const char * const get_data(void)const;
    size_t get_size(void)const;
    void clear(void);
    ~download_buffer(void);
  private:
    size_t m_size;
    char * m_data;
  };
}
#endif /* _DOWNLOAD_BUFFER_H_ */
//EOF
