#include "filesystem.h"
#include "assert.h"
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <cstring> 
using namespace std;

namespace fileIO {
/*
char *LoadTextFile(const std::string &name) { return nullptr; }
char *LoadBinaryFile(const std::string &name) { return nullptr; }
*/

ifstream Searchdirs(const std::string &name) {

  for (const auto s : filedirs) {
    ifstream inFile((s + name).c_str(), ifstream::in);
    if (inFile.good()) {
      return inFile;
    }
  }
  ofstream myfile;
  char e[32];
#ifdef _win32
  LOG(logERROR) << "File Load Error: " << name << " - " << strerror_s(e, 32, errno);
#else
  LOG(logERROR) << "File Load Error: " << name;
#endif
  assert(false);
  return ifstream();
}

const std::string SearchDirsPath(const std::string &name) {
  for (const auto s : filedirs) {
    ifstream inFile((s + name).c_str(), ifstream::in);
    if (inFile.good()) {
      inFile.close();
      return (s + name);
    }
  }
  char e[32];
#ifdef _win32
  LOG(logERROR) << "File Find Error: " << name << " - " << strerror_s(e, 32, errno);
#else
  LOG(logERROR) << "File Load Error: " << name;
#endif
  assert(false);
  return "";
}

bool readFileContents(void *data, const size_t size, FILE *fp) {
  if (!fp || !data)
    return false;

  uint8_t *address = static_cast<uint8_t *>(data);

  size_t bytesRead, totalBytesRead = 0;
  while (totalBytesRead < size) {
    bytesRead = fread(address, 1, size - totalBytesRead, fp);
    if (!bytesRead) {
      return false;
    }

    totalBytesRead += bytesRead;
  }

  return true;
}

BinaryFile *BinaryFile::Load(const std::string &name) {
  BinaryFile *bf = new (BinaryFile);

  bf->data = nullptr;
  bf->size = 0;

  ifstream inFile = Searchdirs(name);
  // copies all data into buffer
  std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), (std::istreambuf_iterator<char>()));
  inFile.close();
  bf->data = new char[buffer.size()];

#if defined(_PLATFORM_X64) || defined(WIN32)
  std::copy(buffer.begin(), buffer.end(), bf->data);
#elif defined(_PLATFORM_PS4)
  std::copy(buffer.begin(), buffer.end(), bf->data);
#endif
  bf->size = buffer.size();

  void *pointer;
  {
    uint32_t bytes;
    const char *filename = "/app0/res/shaders/ps4/compute.sb";
    pointer = NULL;
    bytes = 0;

    bool retValue = false;

    FILE *fp;

#ifdef _win32
    fopen_s(&fp, filename, "rb");
#else
    fp = fopen(filename, "rb");
#endif
    if (fp) {
      if (!fseek(fp, 0, SEEK_END)) {
        bytes = ftell(fp);
        if (!fseek(fp, 0, SEEK_SET)) {
          pointer = malloc(bytes);
          if (pointer) {
            if (readFileContents(pointer, bytes, fp)) {
              retValue = true;
            } else {
              free(pointer);
            }
          }
        }
      }

      fclose(fp);
    }

    if (!retValue) {
      pointer = NULL;
      bytes = 0;
    }
  }
  bf->intdata = (uint32_t *)pointer;
  return bf;
}

Textfile *Textfile::Load(const std::string &name) {

  // Todo: allocator
  Textfile *tf = new Textfile();
  tf->lines = 0;

  vector<string> lines;
  vector<uint32_t> linelenghts;
  uint64_t totalChars = 0;

  ifstream inFile = Searchdirs(name);
  while (!inFile.eof()) {
    string str;
    getline(inFile, str);
    str = str + '\n';
    ++tf->lines;
    totalChars += (uint64_t)str.length();
    linelenghts.push_back((uint32_t)str.length());
    lines.push_back(str);
  }
  inFile.close();

  // squash down to a binary file
  char *rawdata = new char[totalChars];
  uint32_t *ll = new uint32_t[tf->lines];
  for (size_t i = 0; i < tf->lines; i++) {
    ll[i] = linelenghts[i];
  }
  tf->linelengths = ll;
  tf->data.size = totalChars;

  totalChars = 0;
  for (const auto l : lines) {
    memcpy(&rawdata[totalChars], l.c_str(), l.size());
    totalChars += l.length();
  }

  tf->data.data = rawdata;

  return tf;
}

char *Textfile::GetLinePtr(size_t line) {
  assert(line <= lines);
  size_t total = 0;
  for (size_t i = 0; i < line; i++) {
    total += linelengths[i];
  }
  return &data.data[total];
}

std::string Textfile::GetLine(size_t line) {
  char *ptr = GetLinePtr(line);
  string str = "";
  for (size_t i = 0; i < linelengths[line] - 1; i++) {
    str += *(ptr + i);
  }
  return str;
}

bool FileExists(const std::string &name) {
  for (const auto s : filedirs) {
    ifstream inFile((s + name).c_str(), ifstream::in);
    if (inFile.good()) {
      inFile.close();
      return true;
    }
  }
  return false;
}
} // namespace fileIO
