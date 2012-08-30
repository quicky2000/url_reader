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
