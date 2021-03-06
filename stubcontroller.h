/*
    Copyright (c) 2007-2016 Contributors as noted in the AUTHORS file

    This file is part of libstub_helper, the Stub core engine in C++.

    libstub_hepler is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    As a special exception, the Contributors give you permission to link
    this library with independent modules to produce an executable,
    regardless of the license terms of these independent modules, and to
    copy and distribute the resulting executable under terms of your choice,
    provided that you also meet, for each linked independent module, the
    terms and conditions of the license of that module. An independent
    module is a module which is not derived from or based on this library.
    If you modify this library, you must extend this exception to your
    version of the library.

    libstub_hepler is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STUBCONTROLLER_H
#define STUBCONTROLLER_H

class StubController
{
public:
    StubController();
    virtual ~StubController();

    enum DataType
    {
        type_value = 0,
        type_text
    };

    void set(const char* key, int value) const;
    void set(const char* key, void* value, const DataType& type = DataType::type_value) const;
    bool get(const char* key, void** value, const DataType& type = DataType::type_value) const;

    void clear() const;

private:

    enum {
        MAX_FILENAME_LEN =256
    };

    char _path[MAX_FILENAME_LEN];

};

#endif // STUBCONTROLLER_H
