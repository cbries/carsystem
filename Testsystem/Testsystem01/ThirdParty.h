/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * http://www.christianbenjaminries.de
 * https://github.com/cbries
 */

#ifndef __THIRDPARTY_H__
#define __THIRDPARTY_H__

/* Usage:
----
  static char buffer[80];
  if (Serial.available() > 0) 
  {
    if (readline(Serial.read(), buffer, 80) > 0) 
    {
      String s(buffer);
      int value = s.toInt();
      engine.ChangeSpeedTo(value);
    }
  }
----
 */

int readline(int readch, char *buffer, int len)
{
  static int pos = 0;
  int rpos;

  if (readch > 0) {
    switch (readch) {
      case '\r': // Ignore new-lines
        break;
      case '\n': // Return on CR
        rpos = pos;
        pos = 0;  // Reset position index ready for next time
        return rpos;
      default:
        if (pos < len-1) {
          buffer[pos++] = readch;
          buffer[pos] = 0;
        }
    }
  }
  // No end of line has been found, so return -1.
  return -1;
}

#endif // __THIRDPARTY_H__

