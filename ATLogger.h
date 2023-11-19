/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	ATLogger.h
// Author: Jason A. Biddle (JB)
//
// Purpose: A Logger class that can be used during Debugging or for generating Error logs for Atlas Game Engine.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <windows.h>
#include <vector>
#include "CString.h"

#define SET_BIT(x,y) (x |= y)
#define CLEAR_BITS(x) (x = 0)
#define CLEAR_BIT(x,y) (x &= ~y)
#define TOGGLE_BIT(x) (x ^= y)
#define CHECK_BIT(x,y) (((x & y) == y) ? true : false)

namespace Atlas
{
   class CATLogger
   {
      private:
         static CATLogger*       m_pInstance;  //The one and ONLY instance of the Logger.
         HANDLE                  m_hConsole;  //The handle to the Console that we're outputting to.
         CString                 m_sOutputFilename; //Name of the File we're outputting to.
         std::vector<CString>    m_vFileQueue;  //Vector of all the Messages that will be put into file.

         unsigned char           m_cLoggerLevel;  //Message level that we're only outputting.
         unsigned char           m_ucFlags;  //Logger states such as "Are we outputting to a file?" or "Are we viewing a time stamp?".

         //Colors that can be used on Console Output.
         enum eColors {BLACK = 0, BLUE, GREEN, AQUA, RED, PURPLE, YELLOW, WHITE, GRAY, LIGHT_BLUE,
            LIGHT_GREEN, LIGHT_AQUA, LIGHT_RED, LIGHT_PURPLE, LIGHT_YELLO, BRIGHT_WHITE};

         CATLogger();  //Constructor
         CATLogger(const CATLogger&);  //Copy Constructor
         CATLogger* operator=(const CATLogger&);  //Assignment Operator
         ~CATLogger();  //Destructor

         CString buildMessage(const CString &sFormat, va_list args);  //Constructs the Message to be outputted.
         CString ProcessToken(const CString& sToken, va_list &args);  //String-afies a given token into a string.

      public:

         //Various message levels that can be displayed.
         enum eLevel {ERR = 0, WARN, TRACE, INFO, ERR_WARN, ERR_TRACE, ERR_INFO, WARN_TRACE,
            WARN_INFO, TRACE_INFO, ALL};

         //Various flag states for the Logger.
         enum eFlags {TIMESTAMP = 1, LOGFILE = 2, CONSOLE = 4};

         static CATLogger* GetInstance();  //Retrieves the one and ONLY Instance of the Logger.
         static void DeleteInstance();  //Deletes the one and ONLY Instance of the Logger.

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
         bool Init(eLevel Loggerlevel, unsigned char ucFlags, const CString &sOutputFilename = "");
         
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
         void Shutdown();

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
         void info(const CString sMessage, ...);


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
         void trace(const CString sMessage, ...);


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
         void warn(const CString sMessage, ...);


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
         void error(const CString sMessage, ...);

         /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         // Function: SetLevel
         // Last Modified:  November 18th, 2023 (JB)
         // Author:  Jason A. Biddle
         //
         // Purpose:  Set the message level of the Logger.  e.g. SetLevel(eLevel::ERR);
         //
         // In:  level - The type of message(s) that will be outputted by the Logger.
         //              All other message types will be ignored.
         //
         // Out:  None
         /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         void SetLevel(eLevel level = eLevel::INFO) { m_cLoggerLevel = level; }
         
         /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         // Function:  EnableTimeStamp
         // Last Modified:  November 18th, 2023 (JB)
         // Author:  Jason A. Biddle
         //
         // Purpose:  Enables use of a time stamp in messages.
         //
         // In:  None
         //
         // Out:  None
         /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         void EnableTimeStamp() { SET_BIT(m_ucFlags,eFlags::TIMESTAMP); }
         
         /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         // Function:  DisableTimeStamp
         // Last Modified:  November 18th, 2023 (JB)
         // Author:  Jason A. Biddle
         //
         // Purpose:  Disables the use of time stamps in messages.
         //
         // In:  None
         //
         // Out:  None
         /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         void DisableTimeStamp() { CLEAR_BIT(m_ucFlags, eFlags::TIMESTAMP); }
         
         /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         // Function:  SetOutputFile
         // Last Modified:  November 18th, 2023 (JB)
         // Author:  Jason A. Biddle
         //
         // Purpose:  Sets the name of the file that messages will be outputted to.
         //
         // In:  sOutputFilename - Name of the file to be created.
         //
         // Out:  None
         /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         void SetOutputFile(const char* sOutputFilename) { m_sOutputFilename = sOutputFilename; }
   };
}

#ifndef AT_RELEASE
#define AT_LOG_INFO(...)   ::Atlas::CATLogger::GetInstance()->info(__VA_ARGS__);
#define AT_LOG_TRACE(...)  ::Atlas::CATLogger::GetInstance()->trace(__VA_ARGS__);
#define AT_LOG_WARN(...)   ::Atlas::CATLogger::GetInstance()->warn(__VA_ARGS__);
#define AT_LOG_ERROR(...)  ::Atlas::CATLogger::GetInstance()->error(__VA_ARGS__);
#else
#define AT_LOG_INFO(...)
#define AT_LOG_TRACE(...)
#define AT_LOG_WARN(...)
#define AT_LOG_ERROR(...)
#endif

