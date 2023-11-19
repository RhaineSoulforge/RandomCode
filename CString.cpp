#include "CString.h"
#include <string>

CString::CString()
{
   m_pValue = 0;
   m_unLength = 0;
}

CString::CString(const char* pSource)
{
   size_t nSize = strlen(pSource);

   m_pValue = new char[nSize + 1];
   m_unLength = static_cast<int>(nSize);

   int i = 0;
   for (; i < nSize; i++)
      m_pValue[i] = pSource[i];

   m_pValue[nSize] = '\0';
}

CString::CString(const CString& pSource)
{
   size_t nSize = strlen(pSource.m_pValue);

   m_pValue = new char[nSize + 1];
   m_unLength = static_cast<int>(nSize);

   int i = 0;
   for (; i < nSize; i++)
      m_pValue[i] = pSource.m_pValue[i];

   m_pValue[nSize] = '\0';
}

CString::~CString()
{
   if (m_pValue)
      delete[] m_pValue;
}

CString& CString::operator=(const CString& a)
{
   // Same string?  Don't do anything!  Save those Processes!
   if (*this == a)
      return *this;

   delete[] this->m_pValue;
   this->m_pValue = new char[a.m_unLength + 1];

   unsigned int i = 0;
   for (; i < a.m_unLength; i++)
      this->m_pValue[i] = a.m_pValue[i];

   this->m_pValue[a.m_unLength] = '\0';
   this->m_unLength = a.m_unLength;

   return *this;
}

CString& CString::operator=(const char* a)
{
   // Same string?  Don't do anything!  Save those Processes!
   if (*this == a)
      return *this;

   delete[] this->m_pValue;
   this->m_pValue = new char[strlen(a) + 1];
   this->m_unLength = static_cast<int>(strlen(a));

   unsigned int i = 0;
   for (; i < this->m_unLength; i++)
      this->m_pValue[i] = a[i];

   this->m_pValue[this->m_unLength] = '\0';

   return *this;
}

CString& CString::operator=(const char a)
{
   delete[] m_pValue;
   m_pValue = new char[2];
   m_unLength = 1;

   m_pValue[0] = a;
   m_pValue[1] = '\0';

   return *this;
}

const CString CString::operator+(const CString& a)const
{
   CString sResult;

   sResult.m_unLength = this->m_unLength + a.m_unLength;
   sResult.m_pValue = new char[sResult.m_unLength + 1];

   unsigned int i = 0;
   for (; i < this->m_unLength; i++)
      sResult.m_pValue[i] = this->m_pValue[i];

   unsigned int j = 0;
   for (; j < a.m_unLength; j++, i++)
      sResult.m_pValue[i] = a.m_pValue[j];

   sResult.m_pValue[sResult.m_unLength] = '\0';
   return sResult;
}

const CString CString::operator+(const char* a)const
{
   CString sResult;
   unsigned int unSize = static_cast<unsigned int>(strlen(a));

   sResult.m_unLength = this->m_unLength + unSize;
   sResult.m_pValue = new char[sResult.m_unLength + 1];

   unsigned int i = 0;
   for (; i < this->m_unLength; i++)
      sResult.m_pValue[i] = this->m_pValue[i];

   unsigned int j = 0;
   for (; j < unSize; j++, i++)
      sResult.m_pValue[i] = a[j];

   sResult.m_pValue[sResult.m_unLength] = '\0';

   return sResult;
}

const CString CString::operator+(const char a)const
{
   CString sResult;
   sResult.m_unLength = this->m_unLength + 1;
   sResult.m_pValue = new char[sResult.m_unLength + 1];

   unsigned int i = 0;
   for (; i < this->m_unLength; i++)
      sResult.m_pValue[i] = this->m_pValue[i];

   sResult.m_pValue[i] = a;

   sResult.m_pValue[sResult.m_unLength] = '\0';

   return sResult;
}

CString& CString::operator+=(const CString& a)
{
   *this = *this + a;
   return *this;
}

CString& CString::operator+=(const char* a)
{
   *this = *this + a;
   return *this;
}

CString& CString::operator+=(const char a)
{
   *this = *this + a;
   return *this;
}

bool CString::operator==(const char* a) const
{
   int nSize = static_cast<int>(strlen(a));
   
   // Not the same size?  Not the same!
   if (this->m_unLength != nSize)
      return false;

   unsigned int i = 0;
   for (; i < this->m_unLength; i++)
   {
      if (this->m_pValue[i] != a[i])
         return false;
   }

   return true;
}

bool CString::operator==(const CString& a) const
{
   // Not the same size?  Not the same!
   if (this->m_unLength != a.m_unLength)
      return false;

   unsigned int i = 0;
   for (; i < this->m_unLength; i++)
   {
      if (this->m_pValue[i] != a.m_pValue[i])
         return false;
   }

   return true;
}

char& CString::operator[](const int nPosition)
{
   if (nPosition < 0 || nPosition > static_cast<int>(this->m_unLength))
      exit(0);

   return this->m_pValue[nPosition];
}

char& CString::operator[](const int nPostion) const
{
   if (nPostion < 0 || nPostion > static_cast<int>(this->m_unLength))
      exit(0);

   return this->m_pValue[nPostion];
}

std::ostream& operator<<(std::ostream& os, const CString& a)
{
   std::cout << a.getCstr();
   return os;
}

bool CString::Empty() const
{
   if (this->m_pValue && this->m_unLength > 0)
      return false;
   return true;
}

void CString::TrimStart(const char cDelim)
{
   if (m_pValue[0] == cDelim)
   {
      char* temp = m_pValue;

      m_pValue = new char[m_unLength];
      unsigned int i = 1;
      for (; i < (m_unLength); i++)
         m_pValue[i-1] = temp[i];

      m_pValue[m_unLength] = '\0';
      m_unLength--;

      delete[] temp;
   }
}

void CString::TrimEnd(const char cDelim)
{
   if (m_pValue[m_unLength - 1] == cDelim)
   {
      char* temp = m_pValue;

      m_unLength--;
      m_pValue = new char[m_unLength];
      unsigned int i = 0;
      for (; i < m_unLength; i++)
         m_pValue[i] = temp[i];

      delete[] temp;
   }
}

void CString::Trim(const char cDelim)
{
   if ((m_pValue[0] == cDelim) && (m_pValue[m_unLength - 1] == cDelim))
   {
      char* temp = m_pValue;

      int nSize = m_unLength - 2;
      m_pValue = new char[nSize];

      unsigned int i = 1;
      for (; i < (m_unLength - 1); i++)
         m_pValue[i-1] = temp[i];

      m_pValue[nSize] = '\0';
      m_unLength = nSize;
      delete[] temp;
   }
}

void CString::Trim(const char cFront, const char cBack)
{
   if ((m_pValue[0] == cFront) && (m_pValue[m_unLength - 1] == cBack))
   {
      char* temp = m_pValue;

      unsigned int nSize = m_unLength - 2;
      m_pValue = new char[nSize];

      unsigned int i = 1;
      for (; i < (m_unLength - 1); i++)
         m_pValue[i - 1] = temp[i];

      m_pValue[nSize] = '\0';
      m_unLength = nSize;
      delete[] temp;
   }
}

void CString::Remove(unsigned int nEnd, unsigned int nStart)
{
   if (!inBounds(nStart, nEnd))
      return;

   char* sTemp = this->m_pValue;

   unsigned int unSize = this->m_unLength - (nEnd - nStart);

   this->m_pValue = new char[unSize + 1];

   unsigned int i = 0;
   for (; i < nStart; i++)
      this->m_pValue[i] = sTemp[i];

   unsigned int j = nEnd;
   for (; j < this->m_unLength; j++, i++)
      this->m_pValue[i] = sTemp[j];

   this->m_unLength = unSize;
   this->m_pValue[unSize] = '\0';
}

CString CString::Substring(unsigned int nEnd, unsigned int nStart)const
{
   CString sResult;

   if (!inBounds(nStart, nEnd))
      return sResult;

   int nSize = nEnd - nStart;

   if (nSize < 0)
      nSize = 0;

   sResult.m_pValue = new char[nSize + 1];

   unsigned int i = 0, j = nStart;
   for (; j < nEnd; j++, i++)
      sResult.m_pValue[i] = this->m_pValue[j];

   sResult.m_pValue[nSize] = '\0';
   sResult.m_unLength = nSize;

   return sResult;
}

CString CString::Substring(const char cDelim)const
{
   int nResult = this->Find(cDelim);
   if (nResult == -1)
      return CString();
   return this->Substring(static_cast<unsigned int>(nResult));
}

int CString::Find(const char cDelim, unsigned int unStart)const
{
   if (unStart > this->m_unLength)
      return -1;

   unsigned int i = unStart;
   for (; i < this->m_unLength; i++)
   {
      if (this->m_pValue[i] == cDelim)
         return static_cast<int>(i);
   }

   return -1;
}

const char* CString::getCstr() const
{
   return m_pValue;
}

char CString::getChar(unsigned int nLocation)
{
   //Make sure we're inbounds, return a \0 if not!
   if ((nLocation > -1) && (nLocation < this->m_unLength))
      return this->m_pValue[nLocation];
   return '\0';
}

void Copystr(char *arcDestination, const char* arcSource)
{
   size_t nSize = std::strlen(arcSource);
   unsigned int i = 0;
   for (; i < nSize; i++)
      arcDestination[i] = arcSource[i];
   arcDestination[nSize] = '\0';
}

bool CString::inBounds(unsigned int nStart, unsigned int nEnd) const
{
   if (nEnd < nStart)
      return false;
   else if ((nEnd < 0) || (nEnd > this->m_unLength))
      return false;
   else if ((nStart < 0) || (nStart > this->m_unLength))
      return false;

   return true;
}