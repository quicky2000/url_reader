#include "download_buffer.h"
#include <stdlib.h>
#include <iostream>
#include <string.h>

namespace quicky_url_reader
{
  //------------------------------------------------------------------------------
  download_buffer::download_buffer(void):
    m_size(0),
    m_data((char*)malloc(1))
  {
    if(m_data==NULL)
      {
	std::cout << "Not enough memory to allocate buffer" << std::endl;
	exit(EXIT_FAILURE);
      }
  }

  //------------------------------------------------------------------------------
  void download_buffer::clear(void)
  {
    m_size = 0;
    free(m_data);
    m_data = NULL;
  }

  //------------------------------------------------------------------------------
  void download_buffer::add_data(size_t p_size, void * p_data)
  {
    // Extend buffer to the new size
    m_data = (char*)realloc(m_data,m_size + p_size + 1);
    if(m_data == NULL)
      {
	std::cout << "Not enough memory to extend buffer" << std::endl;
	exit(EXIT_FAILURE);      
      }
    // Copy the new data in the buffer
    memcpy(&(m_data[m_size]), p_data, p_size);
    // Update size
    m_size += p_size;
    // Adding NULL chacacter at the end of buffer to be able to print it
    m_data[m_size] = 0;
  }

  //------------------------------------------------------------------------------
  const char * const download_buffer::get_data(void)const
  {
    return m_data;
  }

  //------------------------------------------------------------------------------
  size_t download_buffer::get_size(void)const
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
