#ifndef _URL_READER_H_
#define _URL_READER_H_
#include <curl/curl.h>

namespace quicky_url_reader
{
  class download_buffer;

  class url_reader
  {
  public:
    static url_reader & get_instance(void);
    static void remove_instance(void);
    void read_url(const char * p_url,download_buffer & p_buffer);
    char * escape_string(const char * p_str);
  private:
    /** 
	Private constructor called by singleton to insure that init
	of libcurl will done only once a time
    **/
    url_reader(void);

    /**
       Private constructor called by singleton to insure that ressources
       of libcurl will free only once a time
    **/
    ~url_reader(void);


    static size_t receive_data(void *p_buffer, size_t p_size, size_t p_nmemb, void *p_userp);

    CURL * m_curl_handler;
    static url_reader * m_instance;
  };
}
#endif /* _URL_READER_H_ */
//EOF
