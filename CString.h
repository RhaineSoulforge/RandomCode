#pragma once
#include <iostream>

class CString
{
   private:
      char* m_pValue;
      unsigned int m_unLength;

      bool inBounds(unsigned int nStart, unsigned int nEnd)const;

   public:

      // Constructors
      CString();
      CString(const char* pSource);
      CString(const CString& pSource);

      // Destructor
      ~CString();

      // Operators
      CString& operator=(const CString& a);
      CString& operator=(const char* a);
      CString& operator=(const char a);
      const CString operator+(const CString& a)const;
      const CString operator+(const char* a)const;
      const CString operator+(const char a)const;
      CString& operator+=(const CString& a);
      CString& operator+=(const char* a);
      CString& operator+=(const char a);
      bool operator==(const CString& a)const;
      bool operator==(const char* a)const;
      char& operator[](const int nPosition);
      char& operator[](const int nPosition)const;
      friend std::ostream& operator<<(std::ostream& os, const CString& a);

      // Methods
      inline int Length()const { return this->m_unLength; }
      void Trim(const char cDelim = ' ');
      void Trim(const char cFront, const char cBack);
      void TrimStart(const char cDelim = ' ');
      void TrimEnd(const char cDelim = ' ');
      void Remove(unsigned int nEnd, unsigned int nStart = 0);
      bool Empty()const;
      CString Substring(unsigned int nEnd, unsigned int nStart = 0)const;
      CString Substring(const char cDelim = ' ')const;
      int Find(const char cDelim = ' ', unsigned int unStart = 0)const;
      const char* getCstr()const;
      char getChar(unsigned int nLocation);

   };

//Returns a copy of a C-style string please don't forget to
//delete alocated memory.
void Copystr(char *arcDestination, const char* arcSource);