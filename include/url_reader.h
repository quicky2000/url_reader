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
#ifndef _URL_READER_H_
#define _URL_READER_H_
#ifdef _WIN32
#include "curl/curl.h"
#else
#include <curl/curl.h>
#endif // _WIN32
#include <string>
#include <inttypes.h>

namespace quicky_url_reader
{
  class download_buffer;

  class url_reader
  {
  public:
    /**
       Dump URL Content inside a string
     **/
    void dump_url(const std::string & p_url,std::string & p_result);
    /**
       Dump URL content inside a file opened in text mode
    **/
    void dump_url(const std::string & p_url, const std::string & p_file_name);
    /**
       Dump URL content inside a file opened in binary mode
    **/
    void dump_url_binary(const std::string & p_url, const std::string & p_file_name);
    /**
       Dump URL content inside a download buffer
    **/
    void read_url(const char * p_url,download_buffer & p_buffer);
    /**
       Dump URL content inside a file defined by its file descriptor
    **/
    void dump_url(const std::string & p_url,int fd);
    static char * escape_string(const char * p_str);
    url_reader(void);
    ~url_reader(void);
    static url_reader & instance(void);
    void set_authentication(const std::string & p_proxy,
                            const std::string & p_proxy_port,
                            const std::string & p_user,
                            const std::string & p_password);
  private:
    static size_t receive_data(void *p_buffer,
			       size_t p_size, 
			       size_t p_nmemb,
			       void *p_userp);
    static size_t receive_data_fd(void *p_buffer,
				  size_t p_size, 
				  size_t p_nmemb,
				  void *p_userp);
    static std::string m_proxy;
    static std::string m_proxy_userpwd;
    static CURL * m_curl_handler;
    static url_reader m_instance;
    static uint32_t m_nb_instance;
  };
}
#endif /* _URL_READER_H_ */
//EOF
