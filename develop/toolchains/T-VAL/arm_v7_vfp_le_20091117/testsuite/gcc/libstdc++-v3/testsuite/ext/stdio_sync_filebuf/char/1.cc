// 2003-05-01 Petur Runolfsson <peturr02@ru.is>

// Copyright (C) 2003 Free Software Foundation
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

// { dg-require-fileio "" }

#include <ext/stdio_sync_filebuf.h>
#include <testsuite_hooks.h>

void test01()
{
  using namespace std;

  bool test __attribute__((unused)) = true;
  const char* c_lit = "black pearl jasmine tea";
  int size = strlen(c_lit);
  const char* name = "stdiobuf-1.txt";

  FILE* fout = fopen(name, "w");
  fwrite(c_lit, 1, size, fout);
  fclose(fout);

  FILE* fin = fopen(name, "r");
  __gnu_cxx::stdio_sync_filebuf<char> sbuf(fin);

  VERIFY( sbuf.sgetc() == c_lit[0] );
  VERIFY( getc(fin) == c_lit[0] );
  VERIFY( sbuf.sgetc() == c_lit[1] );
  VERIFY( sbuf.sbumpc() == c_lit[1] );
  VERIFY( ungetc('Z', fin) == 'Z' );
  VERIFY( sbuf.sbumpc() == 'Z' );
  VERIFY( getc(fin) == c_lit[2] );
  VERIFY( sbuf.sputbackc('X') == 'X' );
  VERIFY( getc(fin) == 'X' );

  char buf[5];
  memset(buf, 'x', 5);
  VERIFY( sbuf.sgetn(buf, 5) == 5 );
  VERIFY( !memcmp(buf, c_lit + 3, 5) );
  VERIFY( getc(fin) == c_lit[8] );

  fclose(fin);
}

int main ()
{
  test01();
  return 0;
}
