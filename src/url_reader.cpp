#include "url_reader.h"
#include "download_buffer.h"
#include <iostream>
#include <stdlib.h>

namespace quicky_url_reader
{
  //------------------------------------------------------------------------------
  url_reader::url_reader(void):
    m_curl_handler(NULL)
  {
    std::cout << "Init libcurl" << std::endl ;
    if(curl_global_init(CURL_GLOBAL_ALL))
      {
	std::cout << "Problem with curl initialisation" << std::endl ;
	exit(-1);
      }
    m_curl_handler = curl_easy_init();
  }

  //------------------------------------------------------------------------------
  url_reader::~url_reader(void)
  {
    curl_easy_cleanup(m_curl_handler);
    curl_global_cleanup();
    std::cout << "libcurl cleanup done" << std::endl ;
  }

  //------------------------------------------------------------------------------
  url_reader & url_reader::get_instance(void)
  {
    if(m_instance == NULL)
      {
	m_instance = new url_reader();
      }
    return *m_instance;
  }

  //------------------------------------------------------------------------------
  void url_reader::remove_instance(void)
  {
    delete m_instance;
  }

  //------------------------------------------------------------------------------
  char * url_reader::escape_string(const char * p_str)
  {
    return curl_easy_escape(m_curl_handler,p_str,0);
  }

  //------------------------------------------------------------------------------
  void url_reader::read_url(const char * p_url,download_buffer & p_buffer)
  {
    std::cout << "URL is \"" << p_url << "\"" << std::endl ;
    curl_easy_setopt(m_curl_handler, CURLOPT_URL, p_url);
    curl_easy_setopt(m_curl_handler, CURLOPT_WRITEFUNCTION,url_reader::receive_data); 
    curl_easy_setopt(m_curl_handler, CURLOPT_WRITEDATA, (void *)&p_buffer);
    //  curl_easy_setopt(m_curl_handler, CURLOPT_PROXY, "url:port"); 
    //  curl_easy_setopt(m_curl_handler, CURLOPT_PROXYUSERPWD, "login:motdepasse");
    CURLcode res = curl_easy_perform(m_curl_handler);
    if(res)
      {
	std::cout << "Error when downloading \"" << p_url << "\"" << std::endl ;
      }
  }

  //------------------------------------------------------------------------------
  size_t url_reader::receive_data(void *p_buffer, size_t p_size, size_t p_nmemb, void *p_userp)
  {
    size_t l_real_size = p_size * p_nmemb ;
#ifdef DEBUG_URL_READER
    std::cout << "url_reader::receive data of size " << l_real_size << std::endl ;
#endif
    download_buffer * l_buffer = (download_buffer*) p_userp;
    l_buffer->add_data(l_real_size,p_buffer);
    return p_size*p_nmemb;
  
  }

  url_reader * url_reader::m_instance = NULL;
}
//EOF
