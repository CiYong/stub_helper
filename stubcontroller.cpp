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

#include "stubcontroller.h"

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>

const char* TEMP_FILE_PATH = "./TEMP";
const char* FILE_NAME = "stub.txt";
const char* SEPARATOR = "=";

const int MAX_LINE_SIZE = 1024*20;
const int MAX_KEY_SIZE = 512;
const int MAX_VALUE_SIZE = 768;

static char s_sVal[MAX_VALUE_SIZE] = {0}; // read buffer
static int s_iVal = 0;


StubController::StubController()
{
    memset(&_path, 0x0, MAX_FILENAME_LEN);
    _snprintf(_path, MAX_FILENAME_LEN, "%s\\%s", TEMP_FILE_PATH, FILE_NAME);
    mkdir(TEMP_FILE_PATH);

    memset(s_sVal, 0x0, MAX_VALUE_SIZE);
    s_iVal = 0;
}

StubController::~StubController()
{
    // temp stub path and local path
    remove(_path);
    rmdir(TEMP_FILE_PATH);
}

void StubController::clear() const
{
    FILE* pFile = fopen(_path, "wb");
    if ( !pFile )
    {
        fclose(pFile);
    }

    memset(s_sVal, 0x0, MAX_VALUE_SIZE);
    s_iVal = 0;
}

void StubController::set(const char *key, int value) const
{
    set(key, *reinterpret_cast<void**>(&value));
}

void StubController::set(const char* key, void* value, const DataType &type) const
{
    char keyValue[MAX_VALUE_SIZE] = {0};

    if ( type == DataType::type_text )
    {
        // text , save char to keyvalue
        strncpy(keyValue, (const char*)value, MAX_VALUE_SIZE);
    }
    else
    {
        // text , save int to keyvalue
        sprintf(keyValue, "%d", *reinterpret_cast<int*>(&value));
    }

    char* content = NULL;

    FILE* pFile = fopen(_path, "rb");
    if ( pFile )
    {
        fseek(pFile, 0, SEEK_END);
        size_t length = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
        if ( length )
        {
            content = new char[length + 1];
            memset(content, 0x0, length + 1);
            fread(content, sizeof(char), length, pFile);
        }
        fclose(pFile);
    }

    if ( !content )
    {
        // file empty, write content into file
        pFile = fopen(_path, "wb");
        if ( pFile )
        {
            fprintf(pFile, "%s%s%s\n", key, SEPARATOR, keyValue);
            fclose(pFile);
        }
        return;
    }

    char* pos = content;
    char tempKey[MAX_KEY_SIZE] = {0};
    sprintf(tempKey, "%s%s", key, SEPARATOR);
    bool loopCtrlFlag = false;
    do
    {
        pos = strstr(pos, tempKey);
        if ( !pos )
        {
            break;
        }
        bool result = (pos == content) || (*(pos - 1) == '\n');

        pos += strlen(tempKey);
        if ( result )
        {
            break;
        }
    }
    while( loopCtrlFlag );

    char tempContent[MAX_LINE_SIZE] = {0};
    if ( !pos )
    {
        sprintf( tempContent, "%s%s%s\n%s", key, SEPARATOR, keyValue, content);
    }
    else
    {
        strncpy( tempContent, content, pos - content);
        strcat(tempContent, keyValue);

        while( *pos != '\n' )
        {
            pos++;
        }
        strcat(tempContent, pos);
    }

    // rewrite content into file
    pFile = fopen(_path, "wb");
    if ( pFile )
    {
        fprintf(pFile, "%s", tempContent);
        fclose(pFile);
    }

    if ( content )
    {
        delete[] content;
        content = NULL;
    }
}

bool StubController::get(const char* key, void** value, const DataType& type) const
{
    FILE* pFile = fopen(_path, "rb");

    if ( pFile )
    {
        while( !feof( pFile ) )
        {
            size_t linePos = 0;
            char line[MAX_LINE_SIZE] = {0};
            char char_;
            char_ = (char)fgetc(pFile);
            while ( char_ != '\n' && !feof(pFile) )
            {
                // read by line
                line[linePos++] = char_;
                char_ = (char)fgetc(pFile);
            }

            if ( strlen( line ) )
            {
                char* tempKey = strtok(line, SEPARATOR);
                // check the key exist
                if ( strncmp(key, tempKey, MAX_KEY_SIZE ) == 0 )
                {
                    char* tempValue = strtok(NULL, SEPARATOR);
                    if ( !tempValue )
                    {
                        tempValue = "";
                    }
                    if ( type == DataType::type_text )
                    {
                        memset(s_sVal, 0x0, MAX_VALUE_SIZE);
                        strncpy(s_sVal, tempValue, MAX_VALUE_SIZE);
                        *value = s_sVal;
                    }
                    else
                    {
                        // save to s_iVal
                        s_iVal = atoi(tempValue);
                        *value = &s_iVal;
                    }

                    fclose(pFile);
                    return true;
                }
            }
        }
        fclose(pFile);
    }
    return false;
}
