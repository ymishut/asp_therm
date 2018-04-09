#include "xml_reader.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

// std::pair
#include <utility>

#ifdef _OS_WINDOWS
#include <windows.h>
#endif // _OS_WINDOWS


#ifdef _DEBUG
// buffer of error message (for debuging)
static char errmsg[200];
#endif  // _DEBUG

// =========================================================================
// Struct FileReader
// =========================================================================
// struct for RAII implementation of file 

struct FileReader {
  FILE *file;
  int size;

  FileReader(char *name) {
    if (name == NULL) {
#ifdef _DEBUG
      strcpy(errmsg, "FileReader name == null\0");
#endif  // _DEBUG
      return;
    }
    file = fopen(name, "rb");
    if (file == NULL) {
#ifdef _DEBUG
      strcpy(errmsg, "FileReader cannot find file\0");
#endif  // _DEBUG
      return;
    }
    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    rewind(file);
  }

  ~FileReader() {
    if (file != NULL) {
      fclose(file);
    }
  }

private:
  FileReader(const FileReader &fr);
  FileReader &operator= (const FileReader &fr);
};

XMLReader::XMLReader(char *begin, char *end, bool utf8toCP1251)
  : buffer_begin_(begin), buffer_end_(end), utf8toCP1251_(utf8toCP1251),
    freeing_(false) {
  buffer_size_ = buffer_end_ - buffer_begin_;
  init_data();
}

XMLReader::XMLReader(char *filename, bool utf8toCP1251) 
  : utf8toCP1251_(utf8toCP1251) { 
  FileReader fr(filename);
  buffer_begin_ = new char[fr.size];
  if (fread(buffer_begin_, fr.size, 1, fr.file) != 1) {
    delete[] buffer_begin_;
#ifdef _DEBUG
    strcpy(errmsg, "in readXML 'fread' error\0");
#endif  // _DEBUG
    return;
  }
  buffer_size_ = fr.size;
  buffer_end_ = buffer_begin_ + buffer_size_;
  freeing_ = true;
  init_data();
}

// fill vector "content_" with data
void XMLReader::init_data() {
  char *tmp = buffer_begin_;
  content_.reserve(64);
  while (tmp < buffer_end_) {
    if (*tmp++ == '\n')
      content_.push_back(tmp);
  }
  /* while (tmp != NULL) {
    content_.push_back(tmp);
    // лишняя проверка на конец строки (\0)
    tmp = strchr(tmp, '\n');
  } */
  strs_count_ = content_.size();
  fill_parameters();
}

void XMLReader::fill_parameters() {
  if (!data_.empty())
    return;
  for (int i = 0; i < strs_count_; ++i) {
    // next string for comparable with c++99
    if (content_[i] != NULL)
      data_.insert(std::pair<int, xml_pair>(i, get_pair(content_[i])));
    else 
      data_.insert(std::pair<int, xml_pair>(i, xml_pair()));
  }
  // xml_pair
}

XMLReader *XMLReader::Init(char *filename, bool utf8toCP1251) {
  if (filename == NULL)
    return NULL;
  // return NULL if file not exist
  FileReader fr(filename);
  if (fr.file == NULL)
    return NULL;
  return new XMLReader(filename, utf8toCP1251);
}

XMLReader *XMLReader::Init(char *begin, char *end, bool utf8toCP1251) {
  if (begin == NULL || end == NULL)
    return NULL;
  if (begin >= end)
    return NULL;
  return new XMLReader(begin, end, utf8toCP1251);
}

XMLReader::~XMLReader() {
  if (freeing_)
    delete[] buffer_begin_;
}

char *XMLReader::GetBuffer() {
  return buffer_begin_;
}

int XMLReader::GetBufferSize() {
  return buffer_size_;
}

std::string XMLReader::Find_first_value(const std::string & tag) {
  if (data_.empty())
    assert(0);
  for (int i = 0; i < strs_count_; ++i) {
    if (data_[i].tag == tag)
      return data_[i].value;
  }
  return std::string();
}

std::string XMLReader::Find_last_value(const std::string & tag) {
  if (data_.empty())
    assert(0);
  for (int i = strs_count_ - 1; i <= 0 ; --i) {
    if (data_[i].tag == tag)
      return data_[i].value;
  }
  return std::string();
}

std::string XMLReader::get_tag(char *str) {
  char *end_str = strchr(str, '\n');
  char *beg = strchr(str, '<');
  if (beg == NULL)
    return "";
  else
    ++beg;
  char *end = strchr(beg, '>');
  if (end == NULL || end > end_str)
    return "";
  return std::string(beg, end - beg);
}

XMLReader::xml_pair XMLReader::get_pair(char *data) {
  char *str_end = strchr(data, '\n');
  if (str_end == NULL)
    str_end = buffer_end_;
  std::string tag = get_tag(data);
  if (tag == "")
    return xml_pair();
  if ((tag.size() + 5) > MAX_TAG_LEN)
    return xml_pair();
  char *text_b = strchr(data, '>');
  if (text_b != NULL && text_b != buffer_end_)
    ++text_b;
  else
    return xml_pair();
  // yurii 13_02_2018
  // if xml parameter not setted it can be marked <par_name />
  //   not a <par_name> </par_name>
  if (tag.find('/') != std::string::npos)
    return xml_pair(tag, std::string());
  char *check_tag = text_b - 1;
  while (check_tag > data) {
    if (*check_tag == '/')
      return xml_pair();
    --check_tag;
  }
  // 13_02_2018
  char tag_e[MAX_TAG_LEN];
  sprintf(tag_e, "</%s>", tag.c_str());
  tag_e[MAX_TAG_LEN - 1] = '\0';
  int tag_e_len = strlen(tag_e);
  char *text_e = strchr(text_b, '<');
  if (text_e == NULL || text_e > str_end)
    return xml_pair(tag, std::string());
  // пока указатель не перевалил за указатель на конец
  while (data < buffer_end_) {
    if (strncmp(text_e, tag_e, tag_e_len) == 0) {
      int text_len = text_e - text_b;
      std::string value(text_b, text_len);
      if (utf8toCP1251_)
        return xml_pair(tag, setEncodingCP1251(value));
      else
        return xml_pair(tag, value);
    }
    text_e = strchr(text_e + 1, '<');
  }
#ifdef _DEBUG
  assert(0);
#endif // _DEBUG
  return xml_pair(tag, std::string());
}

#ifdef _OS_WINDOWS
std::string XMLReader::cp1251_encode(const std::wstring &wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(1251, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0 );
    WideCharToMultiByte(1251, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::wstring XMLReader::utf8_decode(const std::string &str) {
  if (str.empty()) return std::wstring();
  int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
  std::wstring wstrTo(size_needed, 0);
  MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
  return wstrTo;
}
#endif  // _OS_WINDOWS
// original encoding of VD100 XML is utf8
//   reset it to cp1251
/// input - string in utf8
std::string XMLReader::setEncodingCP1251(const std::string &utf8_str) {
#ifdef _OS_WINDOWS
  // assert(0)
  // yurii 12_02_2018
  std::wstring wstr = utf8_decode(utf8_str);
  return cp1251_encode(wstr);
#endif  // _OS_WINDOWS
  // else UTF-8 is standart encoding
  assert(0 && "method XMLReader::setEncodingCP1251");
  return std::string();
}
