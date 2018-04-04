/*    This file is part of url_reader
      Copyright (C) 2018  Julien Thevenon ( julien_thevenon at yahoo.fr )

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
#ifdef URL_READER_SELF_TEST

#include "url_reader.h"
#include <iostream>

int main(void)
{
    quicky_url_reader::url_reader & l_instance = quicky_url_reader::url_reader::instance();
    std::string l_page_content;
    l_instance.dump_url("http://www.google.fr", l_page_content);
    std::cout << "Page content :" << std::endl << l_page_content;

}
#endif // URL_READER_SELF_TEST
// EOF
