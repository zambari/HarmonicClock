//
// HA-CL:: HARMONIC CLOCK FIRMWARE Zambari // EQ-AV 2010
// 
// release 8
// other functions.pde
//

        
void blinkCenter()
{
              statusCenterCounter=defTime;
}

void blink1()
{
              status1Counter=defTime;
}

void blink2()
{
              status2Counter=defTime;
}


void buttonsRead()
{
                if (digitalRead(button1)==HIGH) 
                {    delay(20);
                    while (digitalRead(button1)==HIGH) {} // wait until button is released
                    delay(20);
                  blink1();
                  if (globalMode==0) globalMode=-1; else
                  if (globalMode==-1) globalMode=0;
                }
                
                if (digitalRead(button2)==HIGH)
               {  delay(20);
                  while (digitalRead(button2)==HIGH) {} // wait until button is released
                  delay(20);
                  blink2();
                     if (globalMode==-1) { // we are in chann selection mode
                     if (channelMask&(1<<chanCursor)) channelMask=channelMask & ~(1<<chanCursor); else
                                                      channelMask=channelMask  | (1<<chanCursor);
                   }
                     if (globalMode==0)
                     { 
                       if (dispMode==0) dispMode=1; else dispMode=0; 
                       allFade(); // clear all just in case
                     }
                }
                if (digitalRead(button3)==HIGH) 
                {
                   delay(20);
                  while (digitalRead(button3)==HIGH) {} // wait until button is released
                  delay(20);
                     if (globalMode==-1)
                     {  
                         channelMask=(1<<chanCursor); // solo on selected chan
                         blink1();    
                       } else
                       { demoModeFlag=1;
                         demoCounter=5000;
                         demoMode();
                       }
                  }
}


void updateLeds()
{
                  int temp=0;
                  for (int i=0;i<12;i++)
                  {
                   if ((notesL[i]==fullBright)||(notesH[i]==fullBright)) temp=1;
                  }
                  if (temp==1)
                  digitalWrite(statusCenterPin,HIGH);  else
                  
                 if (statusCenterCounter>0)
                 { statusCenterCounter--; 
                    digitalWrite(statusCenterPin,HIGH);  
                 } else
                  {
                    digitalWrite(statusCenterPin,LOW);      
                  }
                  
                  
                 if (status1Counter>0) 
                 { status1Counter--;
                    digitalWrite(statusPin,HIGH);    
                 } else
                  {
                    digitalWrite(statusPin,LOW);      
                  }
                 if (status2Counter>0) 
                 {  status2Counter--;
                    digitalWrite(status2Pin,HIGH);    
                 } else
                  {
                    digitalWrite(status2Pin,LOW);      
                  }
}



void updateOffsets()
{
                offsetCounter++;
                if (offsetCounter>offsetTime)
                {
                 offsetCounter=0;
                offset+=offsetDir; 
                   if (offset>11) offset=0;
                   if (offset<0) offset=11;
                }  
               // displayHigh=analogRead(knob);
}

void strobeTick()
{
                digitalWrite(strobePin,HIGH);
                digitalWrite(strobePin,LOW);
}


void clockTick()
{
                digitalWrite(clockPin,LOW);
                digitalWrite(clockPin,HIGH);
}



void updateDisplay()
{
          //  if offset>
            temp=displayHigh<<offset;
            displayLed=temp%bit12 | ((temp&~(bit12-1))>>12);  // round wrapping lower half
            temp=displayLow<<offset; 
            displayLed=displayLed<<12 | temp%bit12 | ((temp&~(bit12-1))>>12);  // same for upper row
            for (int i=0; i<queueLen;i++)
                {
                     digitalWrite(dataPin,((displayLed>>(queueLen-i-1))&1));
                     clockTick();
                 } 
           strobeTick();
}


void readMidi_Old()
{
             while (Serial.available())
             {
              midiRead=Serial.read();
            
             //  CHECK FOR COMMON MESSAGE TYPES BEGIN
                   if ((midiRead&0xF0)==0x90)
                   {
                         midiStatus=midiRead;  
                         while (Serial.available() == 0) {}            //wait for more data
                         midiParam1=Serial.read();                     //read parameter 1
                        while (Serial.available() == 0) {}             //wait for more data
                         midiParam2=Serial.read();  
                       
                       //if (midiReadPayload()) 
                           handleNoteOn(midiStatus&0x0F,midiParam1,midiParam2); 
                            blink1();
                           Serial.write(midiStatus);
                           Serial.write(midiParam1);
                           Serial.write(midiParam2);
                 } 
                 //else
                 
                 if ((midiRead&0xF0)==0x80)
                 {
                         midiStatus=midiRead;
                        while (Serial.available() == 0) {}            //wait for more data
                         midiParam1=Serial.read();      
                        if (midiParam1&MESSAGEStatus) 
                           {
                             blink2();
                             break;
                           } 
                         while (Serial.available() == 0) {}             //wait for more data
                            midiParam2=Serial.read();                  // wait for param 2 just in case
                          if (midiParam1&MESSAGEStatus) 
                           {
                             blink2();
                             break;
                           } 
                           handleNoteOff(midiStatus&0x0F,midiParam1,midiParam2);          //read parameter 1
                         //  Serial.write(midiStatus);
                         //  Serial.write(midiParam1);
                         //  Serial.write(midiParam2);
                  }
             /*
                   if ((midiRead&0xF0)==MESSAGEPitch)
                     {
                           midiStatus=midiRead;  
                           while (Serial.available() == 0) {}            //wait for more data
                           midiParam1=Serial.read();                     //read parameter 1
                           while (Serial.available() == 0) {}             //wait for more data
                           midiParam2=Serial.read();  
                           Serial.write(midiStatus);
                           Serial.write(midiParam1);
                           Serial.write(midiParam2);
                          // handleNoteOn(midiStatus&0x0F,midiParam1,midiParam2);         
                     } 
                     if ((midiRead&0xF0)==MESSAGECC)
                     {
                           midiStatus=midiRead;  
                           while (Serial.available() == 0) {}            //wait for more data
                           midiParam1=Serial.read();                     //read parameter 1
                           while (Serial.available() == 0) {}             //wait for more data
                           midiParam2=Serial.read();  
                           Serial.write(midiStatus);
                           Serial.write(midiParam1);
                           Serial.write(midiParam2);
                          // handleNoteOn(midiStatus&0x0F,midiParam1,midiParam2);         
                     } 
             */
             // check for common ends
                 } // while serial
 }

 
  
  
  
  
  
