/**
 * Copyright (c) 2017 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Narciso Rubio
 * Creation date: 03/31/2017
 */

#ifndef BUZZ_h 
#define BUZZ_h 

# include "Arduino.h"

/**
 * Class: BUZZ
 * 
 * Description:
 * Class to control the buzzer
 */
class BUZZ
{
  /**
   * Attribute declaration
   */
  private:
    /**
     * Array of frequencies
     */
    const uint16_t _notes[12] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440,466, 494}; 
    
    /**
     * Output pin for the buzzer
     */    
    uint8_t _pin; 
   
  public:
    /**
     * Class constructor
     * 
     * @param output pin for the buzzer
     */
    inline BUZZ (uint8_t pin)
    {
       pinMode(pin, OUTPUT);
       _pin = pin;
    }

    /**
     * play
     * 
     * Function to sound the buzzer
     * 
     * @param Array position is between 0-11 to indicate a frequency
     */    
    inline void play (uint8_t note )
    {
      tone(_pin, _notes[note]);
    }
    
    /**
     * stop
     * 
     * Function that for the buzzer
     */ 
    inline void stop(void)
    {
      noTone(_pin);
    }
};

#endif
