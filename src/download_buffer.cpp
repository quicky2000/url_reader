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
#include "download_buffer.h"
#include "quicky_exception.h"
#include <stdlib.h>
#include <iostream>
#include <string.h>

namespace quicky_url_reader
{
    //------------------------------------------------------------------------------
    download_buffer::download_buffer(void):
            m_size(0),
            m_data((char *) malloc(1024))
    {
        if (m_data == NULL)
        {
            throw quicky_exception::quicky_runtime_exception("Not enough memory to allocate download buffer",
                                                             __LINE__,
                                                             __FILE__
                                                            );
        }
    }

    //------------------------------------------------------------------------------
    void
    download_buffer::clear(void)
    {
        m_size = 0;
        free(m_data);
        m_data = NULL;
    }

    //------------------------------------------------------------------------------
    void
    download_buffer::add_data(size_t p_size,
                              void *p_data
                             )
    {
        // Extend buffer to the new size
        m_data = (char *) realloc(m_data,
                                  m_size + p_size + 1
                                 );
        if (m_data == NULL)
        {
            throw quicky_exception::quicky_runtime_exception("Not enough memory to extend download buffer",
                                                             __LINE__,
                                                             __FILE__
                                                            );
        }
        // Copy the new data in the buffer
        memcpy(&(m_data[m_size]),
               p_data,
               p_size
              );
        // Update size
        m_size += p_size;
        // Adding NULL chacacter at the end of buffer to be able to print it
        m_data[m_size] = 0;
    }

    //------------------------------------------------------------------------------
    const char *const
    download_buffer::get_data(void) const
    {
        return m_data;
    }

    //------------------------------------------------------------------------------
    size_t
    download_buffer::get_size(void) const
    {
        return m_size;
    }

    //------------------------------------------------------------------------------
    download_buffer::~download_buffer(void)
    {
        free(m_data);
    }
}
//EOF
