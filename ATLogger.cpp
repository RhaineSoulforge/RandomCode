#include "ATLogger.h"
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>

//The one and ONLY!!!!
Atlas::CATLogger* Atlas::CATLogger::m_pInstance = 0;

//Size of cBuffer used in snprintf (in ProcessToken)
#define BUFFER_SIZE 256

namespace Atlas
{
   //Constructor
   CATLogger::CATLogger()
   {
      this->m_hConsole = 0;  //Sets Handler to null.
      this->m_cLoggerLevel = eLevel::ALL;  //Set default level to all messages.
      m_ucFlags = 0;  //Sets all flags to off.
   }

   //Deconstructor
   CATLogger::~CATLogger()
   {
      this->Shutdown();
   }

   //Retrieve the ONLY instance of ATLogger.
   CATLogger* CATLogger::GetInstance()
   {
      //Have I already been allocated?
      if (!m_pInstance)
         m_pInstance = new CATLogger();  //No?  Go ahead an allocate.
      return m_pInstance;
   }

   //Delete instance of ATLogger.
   void CATLogger::DeleteInstance()
   {
      //Was I created?
      if (m_pInstance)
         delete m_pInstance;  //Then delete me!
   }

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Function: Init
   // Last Modified:  November 18th, 2023 (JB)  
   // Author:  Jason A. Biddle
   //
   // Purpose:  Intializes the Logger
   //
   // In:  Loggerlevel - The type of message(s) that the Logger will display or output to file.
   //      ucFlags - Set whether we're using a Time Stamp, Outputting to Console and/or file.
   //      sOutputFilename - Name of the file for where the messages will be ouputted.
   // 
   // Out:  Returns true if Logger was successfully intialized, false otherwise.
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   bool CATLogger::Init(eLevel Loggerlevel, unsigned char ucFlags, const CString& sOutputFilename)
   {
      //If we're not a console app already then we need to create a console.
      #ifdef AT_WINDOWS
         bool bresult = AllocConsole();
         if (!bresult)
            return false;
      #endif

      this->SetLevel(Loggerlevel);
      SET_BIT(m_ucFlags, ucFlags);

      //Are we printing to Console?
      if (CHECK_BIT(m_ucFlags, eFlags::CONSOLE))
      {
         FILE* pStream;
         errno_t etResult = freopen_s(&pStream, "CONOUT$", "w", stdout);
         if (etResult != 0)
            return false;

         //Grab the Handle to the Console for future use.
         this->m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      }

      //Are we outputting to a file, if so store the name of that file.
      if (CHECK_BIT(m_ucFlags, eFlags::LOGFILE))
         SetOutputFile(sOutputFilename.getCstr());

      //We're in the clear output message saying so and telling user what message level was set.
      this->info("Atlas Logger Intialized at level {i}", this->m_cLoggerLevel);

      return true;
   }

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Function: Shutdown
   // Last Modified:  November 18th, 2023 (JB)
   // Author:  Jason A. Biddle
   //
   // Purpose:  Cleans up memory and writes out messages to desired output file.
   //
   // In:  None
   //
   // Out:  None
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////         
   void CATLogger::Shutdown()
   {
      //If we're not a console app then we need to free up the Console we created.
      #ifdef AT_WINDOWS
         FreeConsole();
      #endif

      //Are we outputting to a file and there are message to be outputted to that file?
      if (CHECK_BIT(m_ucFlags,eFlags::LOGFILE) && !m_vFileQueue.empty())
      {
         std::ofstream fout(m_sOutputFilename.getCstr());
         std::vector<CString>::iterator iter = m_vFileQueue.begin();
         std::vector<CString>::iterator iterEnd = m_vFileQueue.end();
         while (iter != iterEnd)
         {
            fout.write((*iter).getCstr(), (*iter).Length());
            fout.write("\n", 1);
            iter++;
         }
         fout.close();
      }
   }

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Function:  info
   // Last Modified:  November 18th, 2023 (JB)
   // Author:  Jason A. Biddle
   //
   // Purpose:  Prints out information messages to Console and/or file, if printed to Console these messages
   //           print out in Aqua color.
   //
   // In:  sMessage - Message to be printed.
   //      ... - variables to be imprinted into sMessage using {} e.g. info("There are {i} items in array.",nCount)
   //
   // Out:  None
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   void CATLogger::info(const CString sMessage, ...)
   {
      //Are we displaying this message?  If not get outta here!
      if (this->m_cLoggerLevel != eLevel::INFO && this->m_cLoggerLevel != eLevel::ERR_INFO
         && this->m_cLoggerLevel != eLevel::WARN_INFO && this->m_cLoggerLevel != eLevel::TRACE_INFO
         && this->m_cLoggerLevel != eLevel::ALL)
         return;

      va_list args;  //Grab the list of arguments passed to info.
      va_start(args, sMessage);  //Begin processing from args list.

      CString sResult = this->buildMessage(sMessage, args);
      if (CHECK_BIT(m_ucFlags,eFlags::LOGFILE))  //If we're outputting to file then store in vector for later processing.
         m_vFileQueue.push_back(sResult);

      va_end(args);  //Done with the arg list.

      //Are we printing to console?  Then print it out in Aqua color.
      if (CHECK_BIT(m_ucFlags, eFlags::CONSOLE))
      {
         SetConsoleTextAttribute(this->m_hConsole, eColors::AQUA);
         std::cout << sResult << std::endl;
         SetConsoleTextAttribute(this->m_hConsole, eColors::WHITE);
      }
   }

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Function:  trace
   // Last Modified:  November 18th, 2023 (JB)
   // Author:  Jason A. Biddle
   //
   // Purpose:  Prints out trace messages to Console and/or file, if printed to Console these messages
   //           print out in Aqua color.
   //
   // In:  sMessage - Message to be printed.
   //      ... - variables to be imprinted into sMessage using {} e.g. info("There are {i} items in array.",nCount)
   //
   // Out:  None
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   void CATLogger::trace(const CString sMessage, ...)
   {
      //Are we displaying this message?  If not get outta here!
      if (this->m_cLoggerLevel != eLevel::TRACE && this->m_cLoggerLevel != eLevel::ERR_TRACE
         && this->m_cLoggerLevel != eLevel::WARN_TRACE && this->m_cLoggerLevel != eLevel::TRACE_INFO
         && this->m_cLoggerLevel != eLevel::ALL)
         return;

      va_list args;  //Grab the list of arguments passed to info.
      va_start(args, sMessage);  //Begin processing from args list.

      CString sResult = this->buildMessage(sMessage, args);
      if (CHECK_BIT(m_ucFlags, eFlags::LOGFILE))  //If we're outputting to file then store in vector for later processing.
         m_vFileQueue.push_back(sResult);

      va_end(args);  //Done with the arg list.

      //Are we printing to console?  Then print it out in Green color.
      if (CHECK_BIT(m_ucFlags, eFlags::CONSOLE))
      {
         SetConsoleTextAttribute(this->m_hConsole, eColors::GREEN);
         std::cout << sResult << std::endl;
         SetConsoleTextAttribute(this->m_hConsole, eColors::WHITE);
      }
   }

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Function:  warn
   // Last Modified:  November 18th, 2023 (JB)
   // Author:  Jason A. Biddle
   //
   // Purpose:  Prints out warning messages to Console and/or file, if printed to Console these messages
   //           print out in Aqua color.
   //
   // In:  sMessage - Message to be printed.
   //      ... - variables to be imprinted into sMessage using {} e.g. info("There are {i} items in array.",nCount)
   //
   // Out:  None
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   void CATLogger::warn(const CString sMessage, ...)
   {
      //Are we displaying this message?  If not get outta here!
      if (this->m_cLoggerLevel != eLevel::WARN && this->m_cLoggerLevel != eLevel::ERR_WARN
         && this->m_cLoggerLevel != eLevel::WARN_INFO && this->m_cLoggerLevel != eLevel::WARN_TRACE
         && this->m_cLoggerLevel != eLevel::ALL)
         return;

      va_list args;  //Grab the list of arguments passed to info.
      va_start(args, sMessage);  //Begin processing from args list.

      CString sResult = this->buildMessage(sMessage, args);
      if (CHECK_BIT(m_ucFlags, eFlags::LOGFILE))  //If we're outputting to file then store in vector for later processing.
         m_vFileQueue.push_back(sResult);

      va_end(args);  //Done with the arg list.

      //Are we printing to console?  Then print it out in Yellow color.
      if (CHECK_BIT(m_ucFlags, eFlags::CONSOLE))
      {
         SetConsoleTextAttribute(this->m_hConsole, eColors::YELLOW);
         std::cout << sResult << std::endl;
         SetConsoleTextAttribute(this->m_hConsole, eColors::WHITE);
      }
   }

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Function:  error
   // Last Modified:  November 18th, 2023 (JB)
   // Author:  Jason A. Biddle
   //
   // Purpose:  Prints out error messages to Console and/or file, if printed to Console these messages
   //           print out in Aqua color.
   //
   // In:  sMessage - Message to be printed.
   //      ... - variables to be imprinted into sMessage using {} e.g. info("There are {i} items in array.",nCount)
   //
   // Out:  None
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   void CATLogger::error(const CString sMessage, ...)
   {
      //Are we displaying this message?  If not get outta here!
      if (this->m_cLoggerLevel != eLevel::ERR && this->m_cLoggerLevel != eLevel::ERR_INFO
         && this->m_cLoggerLevel != eLevel::ERR_WARN && this->m_cLoggerLevel != eLevel::ERR_TRACE
         && this->m_cLoggerLevel != eLevel::ALL)
         return;

      va_list args;  //Grab the list of arguments passed to info.
      va_start(args, sMessage);  //Begin processing from args list.

      CString sResult = this->buildMessage(sMessage, args);
      if (CHECK_BIT(m_ucFlags, eFlags::LOGFILE))  //If we're outputting to file then store in vector for later processing.
         m_vFileQueue.push_back(sResult);

      va_end(args);  //Done with the arg list.

      //Are we printing to console?  Then print it out in Red color.
      if (CHECK_BIT(m_ucFlags, eFlags::CONSOLE))
      {
         SetConsoleTextAttribute(this->m_hConsole, eColors::RED);
         std::cout << sResult << std::endl;
         SetConsoleTextAttribute(this->m_hConsole, eColors::WHITE);
      }
   }

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Function:  buildMessage
   // Last Modified:  November 18th, 2023 (JB)
   // Author:  Jason A. Biddle
   //
   // Purpose:  Constructs the message being outputted by the Logger.
   //
   // In:  sFormat - The message being outputted e.g. "Hello there {f}!"
   //      args - The variables that will be pressed into sFormat.
   //
   // Out:  sResult - The completed message to be outputted.
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   
   CString CATLogger::buildMessage(const CString &sFormat, va_list args)
   {
      CString sResult;  //The completed message.

      //Are we using a time stamp?  If so add it to sResult.
      if (CHECK_BIT(m_ucFlags,eFlags::TIMESTAMP))
      {
         time_t ttCurrent = time(0);
         tm now_time = {};

         char cBuffer[30] = {};

         localtime_s(&now_time, &ttCurrent);  //Grabs current system time.
         strftime(cBuffer, 30, "[%D %T]", &now_time);  //Format it into something legable.
         sResult += cBuffer;
         sResult += ' ';
      }

      int i = 0;
      int nLastPos = 0;  //Last position of } found.
      bool bCarryOn = true;  //Keep going?
      bool bFirstPass = true;  //Have we already found a {}?
      while(bCarryOn)
      {
         switch (sFormat[i])
         {
            case '\0':  //We've reach end of string.
            {
               bCarryOn = false;  //Stop processing.
               if (bFirstPass)  //Didn't find any tags?  Then sFormat is sResult.
                  sResult += sFormat;
               else
                  sResult += sFormat.Substring(i, nLastPos);
               break;
            }
            case '{':
            {
               sResult += sFormat.Substring(i, nLastPos);  //Grab the substring from last tag to new tag.

               //Find end of tag.
               int j = i;
               while (sFormat[j] != '}')
                  j++;               

               //Break out the variable type.
               CString sToken = sFormat.Substring(j, i+1);

               if (bFirstPass)
                  bFirstPass = false;

               sResult += ProcessToken(sToken, args);
               nLastPos = j + 1;
               break;
            }
         }

         i++;
      }

      return sResult;
   }

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Function:  ProcessToken
   // Last Modified:  November 18th, 2023 (JB)
   // Author:  Jason A. Biddle
   //
   // Purpose:  Grabs the variable out of the args list to covert into a string.
   //
   // In:  sToken - The type the variable is. e.g. i = int, u = unsigned int, etc.
   //      args - List of arguments set into the buildMessage function.
   //
   // Out: sResult - The variable turned into a CString.
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   
   CString CATLogger::ProcessToken(const CString &sToken, va_list &args)
   {
      CString sResult;  //Converted variable into string.
      char cBuffer[256] = {};  //Buffer used for snprintf.
      int nOut = 0;  //Number of character used by snprintf.

      if (sToken.Length() == 1)
      {
         switch (sToken[0])
         {
            case 'i':  // int/short
            {
               sResult = std::to_string(va_arg(args, int)).c_str();
               break;
            }
            case 'u':  // Unsigned int/short/char
            {
               sResult = std::to_string(va_arg(args, unsigned int)).c_str();
               break;
            }
            case 'f':  //Float
            {
               sResult = std::to_string(va_arg(args, double)).c_str();
               break;
            }
            case 'd':  //Double
            {
               sResult = std::to_string(va_arg(args, double)).c_str();
               break;
            }
            case 'c':  //Single character
            {
               sResult = va_arg(args, char);
               break;
            }
            case 's':  //String
            {
               sResult = va_arg(args, const char*);
               break;
            }
            case 'p':  //Printing out pointer address.
            {
               nOut = snprintf(cBuffer, BUFFER_SIZE, "0x%p", va_arg(args, void*));
               sResult = cBuffer;
               break;
            }
            case 'b':  //Boolean
            {
               bool bValue = va_arg(args, bool);
               if (bValue)
                  sResult = "True";
               else
                  sResult = "False";
               break;
            }
            case 'l':  //Long
            {
               sResult = std::to_string(va_arg(args, long int)).c_str();
               break;
            }
         }
      }
      else
      {
         if (sToken == "ll")  //Long Long
            sResult = std::to_string(va_arg(args, long long int)).c_str();
         else if (sToken == "lu")  //unsigned Long
            sResult = std::to_string(va_arg(args, unsigned long int)).c_str();
         else if (sToken == "llu")  //unsigned Long Long
            sResult = std::to_string(va_arg(args,unsigned long long int)).c_str();
      }
      return sResult;
   }
}