/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	 CString.h
// Author:  Jason A. Biddle
//
// Purpose:  A Java style string written in C++.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <iostream>

class CString
{
   private:
      char* m_pValue;  //The string
      unsigned int m_unLength;  //The length of the string.

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Function:  inBounds
      // Last Modified:  November 19th, 2023 (JB)
      // Author:  Jason A. Biddle
      //
      // Purpose:  Checks to make sure that the two given points are within the bounds of the string.
      //
      // In:  nStart - The head of the boundary.
      //      nEnd - The tail of the boundary.
      //
      // Out:  true if in bounds, false otherwise.
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Function:  Length
      // Last Modified:  November 20th, 2023 (JB)
      // Author:  Jason A. Biddle
      //
      // Purpose:  Returns the length of the string.
      //
      // In:  None
      //
      // Out:  Length of the string.
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      inline int Length()const { return this->m_unLength; }

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Function:  Trim
      // Last Modified:  November 20th, 2023 (JB) 
      // Author:  Jason A. Biddle
      //
      // Purpose:  Trims the specified character from the head and tail of the string.
      //
      // In:  cDelim - The character to remove, defaults to a space.
      //
      // Out:  None
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      void Trim(const char cDelim = ' ');

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Function:  Trim
      // Last Modified:  November 20th, 2023 (JB) 
      // Author:  Jason A. Biddle
      //
      // Purpose:  Removes the specified characters from the front and back of the string.
      //
      // In:  cFront - The character to be removed from the front.
      //      cBack - The character to be removed from the back.
      //
      // Out:  None
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      void Trim(const char cFront, const char cBack);
      
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Function:  TrimStart
      // Last Modified:  November 20th, 2023 (JB)
      // Author:  Jason A. Biddle
      //
      // Purpose:  Removes the specified character from the front of the string.
      //
      // In:  cDelim - The character to remove, defaults to a space.
      //
      // Out:  None
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      void TrimStart(const char cDelim = ' ');
      
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Function:  TrimEnd
      // Last Modified:  November 20th, 2023 (JB)
      // Author:  Jason A. Biddle
      //
      // Purpose:  Removes the specified character from the end of the string.
      //
      // In:  cDelim - The character to remove, defaults to a space.
      //
      // Out:  None
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      void TrimEnd(const char cDelim = ' ');
      
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Function:  Remove
      // Last Modified:  November 20th, 2023 (JB)
      // Author:  Jason A. Biddle
      //
      // Purpose:  Remove a section of the string.
      //
      // In:  nEnd - The end of the section of the string to be removed.
      //      nStart - The beginning of the section of the string to be removed, defaults to start of string.
      //
      // Out:  None
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      void Remove(unsigned int nEnd, unsigned int nStart = 0);

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Function:  Empty
      // Last Modified:  November 20th, 2023 (JB)
      // Author:  Jason A. Biddle
      //
      // Purpose:  Checks to see if the string is empty.
      //
      // In:  None
      //
      // Out:  true if it is empty, false otherwise.
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      bool Empty()const;

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Function:  Substring
      // Last Modified:  November 20th, 2023 (JB)
      // Author:  Jason A. Biddle
      //
      // Purpose:  Returns the substring specified by End and Start points.
      //
      // In:  nEnd - The end of the substring.
      //      nStart - The start of the substring, defaults to beginning of the string.
      //
      // Out:  The desired substring.
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      CString Substring(unsigned int nEnd, unsigned int nStart = 0)const;

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Function:  Substring
      // Last Modified:  November 20th, 2023 (JB)
      // Author:  Jason A. Biddle
      //
      // Purpose:  Returns a substring ending at the first instance of a specified character.
      //
      // In:  cDelim - The stopping point for our substring, defaults to a space.
      //
      // Out:  Returns the substring that starts at 0 and ends at first instance of cDelim.
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      CString Substring(const char cDelim = ' ')const;
      
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Function:  Find
      // Last Modified:  November 20th, 2023 (JB) 
      // Author:  Jason A. Biddle
      //
      // Purpose:  Returns the position of a specified character at a specified starting point.
      //
      // In:  cDelim - The character we're looking for.
      //      unStart - Where we start the search, defaults to the start of the string.
      //
      // Out:  The position of the first instance of specified character.
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      int Find(const char cDelim = ' ', unsigned int unStart = 0)const;

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Function:  getCstr
      // Last Modified:  November 20th, 2023 (JB)
      // Author:  Jason A. Biddle
      //
      // Purpose:  Returns a C-style string.
      //
      // In:  None
      //
      // Out:  Returns the C-style string of this CString.
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      const char* getCstr()const;
      
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Function:  getChar
      // Last Modified:  November 20th, 2023 (JB)
      // Author:  Jason A. Biddle
      //
      // Purpose:  Returns the character at the specified location within the string.
      //
      // In:  nLocation - The position in the string.
      //
      // Out:  The character at the specified location.
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      char getChar(unsigned int nLocation);

   };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function:  Copystr
// Last Modified:  November 20th, 2023 (JB)
// Author:  Jason A. Biddle
//
// Purpose:  Returns a copy of a C-style string please don't forget to delete alocated memory.
//
// In:  arcDestination - The character buffer that we're copying into.
//      arcSource - The string that we're copying.
//
// Out:  None.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Copystr(char *arcDestination, const char* arcSource);