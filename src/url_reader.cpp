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
#include "url_reader.h"
#include "quicky_exception.h"
#include "download_buffer.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <unistd.h>

namespace quicky_url_reader
{
  //------------------------------------------------------------------------------
  url_reader::url_reader(void)
  {
    if(!m_nb_instance)
      {
#ifdef DEBUG_URL_READER
        std::cout << "Init libcurl" << std::endl ;
#endif
        if(curl_global_init(CURL_GLOBAL_ALL))
          {
	    throw quicky_exception::quicky_runtime_exception("Problem with curl initialisation",__LINE__,__FILE__);
          }
        m_curl_handler = curl_easy_init();
      }
    ++m_nb_instance;
  }

  //------------------------------------------------------------------------------
  void url_reader::set_authentication(const std::string & p_proxy,
                          const std::string & p_proxy_port,
                          const std::string & p_user,
                          const std::string & p_password)
  {
    m_proxy = p_proxy+":"+p_proxy_port;
    m_proxy_userpwd = p_user+":"+p_password ;
    curl_easy_setopt(m_curl_handler, CURLOPT_PROXY, m_proxy.c_str());
    curl_easy_setopt(m_curl_handler, CURLOPT_PROXYUSERPWD,m_proxy_userpwd.c_str());
  }

  //------------------------------------------------------------------------------
  url_reader::~url_reader(void)
  {
    --m_nb_instance;
    if(!m_nb_instance)
      {
        curl_easy_cleanup(m_curl_handler);
        curl_global_cleanup();
#ifdef DEBUG_URL_READER
        std::cout << "libcurl cleanup done" << std::endl ;
#endif
      }
  }

  //------------------------------------------------------------------------------
  char * url_reader::escape_string(const char * p_str)
  {
    return curl_easy_escape(m_curl_handler,p_str,0);
  }

  //------------------------------------------------------------------------------
  void url_reader::read_url(const char * p_url,download_buffer & p_buffer)
  {
    curl_easy_setopt(m_curl_handler, CURLOPT_WRITEFUNCTION,url_reader::receive_data); 
    curl_easy_setopt(m_curl_handler, CURLOPT_URL, p_url);
    curl_easy_setopt(m_curl_handler, CURLOPT_WRITEDATA, (void *)&p_buffer);
    CURLcode res = curl_easy_perform(m_curl_handler);
    if(res)
      {
	std::stringstream l_stream;
	l_stream << "Error when downloading \"" << p_url << "\"" ;
	throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
      }
  }

  //------------------------------------------------------------------------------
  void url_reader::dump_url(const std::string & p_url,std::string & p_result)
  {
    download_buffer l_buffer;
    this->read_url(p_url.c_str(),l_buffer);
    p_result = std::string(l_buffer.get_data(),l_buffer.get_size());
  }

  //------------------------------------------------------------------------------
  void url_reader::dump_url(const std::string & p_url,const std::string & p_file_name)
  {
    download_buffer l_buffer;
    this->read_url(p_url.c_str(),l_buffer);
    std::ofstream l_output_file(p_file_name.c_str());
    if(!l_output_file.is_open())
      {
	std::stringstream l_stream;
	l_stream << "ERROR : Unable to open output file \"" << p_file_name << "\"" ;
	throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
      }
    l_output_file.write(l_buffer.get_data(),l_buffer.get_size());
    l_output_file.close();
  }

 //------------------------------------------------------------------------------
  void url_reader::dump_url_binary(const std::string & p_url,const std::string & p_file_name)
  {
    download_buffer l_buffer;
    this->read_url(p_url.c_str(),l_buffer);
    std::ofstream l_output_file(p_file_name.c_str(),std::ios::out | std::ios::binary);
    if(!l_output_file.is_open())
      {
	std::stringstream l_stream;
	l_stream << "ERROR : Unable to open output file \"" << p_file_name << "\"" ;
	throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
      }
    l_output_file.write(l_buffer.get_data(),l_buffer.get_size());
    l_output_file.close();
  }

  //------------------------------------------------------------------------------
  void url_reader::dump_url(const std::string & p_url,int fd)
  {
    curl_easy_setopt(m_curl_handler, CURLOPT_WRITEFUNCTION,url_reader::receive_data_fd); 
    curl_easy_setopt(m_curl_handler, CURLOPT_URL, p_url.c_str());
    curl_easy_setopt(m_curl_handler, CURLOPT_WRITEDATA, (uintptr_t)fd);
    CURLcode res = curl_easy_perform(m_curl_handler);
    if(res)
      {
	std::stringstream l_stream;
	l_stream << "Error when downloading \"" << p_url << "\"" ;
	throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
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

  //------------------------------------------------------------------------------
  size_t url_reader::receive_data_fd(void *p_buffer, size_t p_size, size_t p_nmemb, void *p_userp)
  {
    size_t l_real_size = p_size * p_nmemb ;
#ifdef DEBUG_URL_READER
    std::cout << "url_reader::receive data of size " << l_real_size << std::endl ;
#endif
    int fd = (intptr_t) p_userp;
    write(fd,p_buffer,l_real_size);;
    return p_size*p_nmemb;
  
  }

  //------------------------------------------------------------------------------
  url_reader & url_reader::instance(void)
  {
    return m_instance;
  }

  std::string url_reader::m_proxy;
  std::string url_reader::m_proxy_userpwd;
  CURL * url_reader::m_curl_handler = NULL;
  uint32_t url_reader::m_nb_instance = 0;
  url_reader url_reader::m_instance;
}
//EOF
