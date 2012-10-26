#ifndef _URL_READER_H_
#define _URL_READER_H_
#include <curl/curl.h>
#include <string>
#include <inttypes.h>

namespace quicky_url_reader
{
  class download_buffer;

  class url_reader
  {
  public:
    void read_url(const char * p_url,download_buffer & p_buffer);
    char * escape_string(const char * p_str);
    url_reader(void);
    ~url_reader(void);
    void set_authentication(const std::string & p_proxy,
                            const std::string & p_proxy_port,
                            const std::string & p_user,
                            const std::string & p_password);
  private:
    static size_t receive_data(void *p_buffer, size_t p_size, size_t p_nmemb, void *p_userp);
    static std::string m_proxy;
    static std::string m_proxy_userpwd;
    static CURL * m_curl_handler;
    static url_reader m_instance;
    static uint32_t m_nb_instance;
  };
}
#endif /* _URL_READER_H_ */
//EOF
