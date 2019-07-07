
//
// HA-CL:: HARMONIC CLOCK FIRMWARE Zambari // EQ-AV 2010
// 
// release 8
//
// todo: doesn't like short note-off messages or strings of cc's

int clockPin=3;
int strobePin=4;
int statusPin=16; // red
int status2Pin=15; // green 
int statusCenterPin=14; // center
int statusCenterCounter=0;
int status1Counter=0;
int status2Counter=0;
int defTime=200;
int button1=5;
int button2=6;
int button3=7;
int knob=4;
int notesH[13];
int notesL[13];
int fadeValues[12]={0,1,2,3,4,5,6,7,8,12,32,32};
int PWMlen=32;
int fullBright=11;
int startFade=10;
int midiRead;
int PWMcounter=0;
long int lastMsIface;
long int lastMsFade;
long int lastRandomNoteMs=0;
int globalMode=0;
unsigned long channelMask=65535; // 1111 1111 1111 111 all channels
//unsigned long channelMask=4080; // 0000 1111 1111 0000
int lastAnalogRead;
int currentAnalogRead;
int blinkCounter=0;
int blinkTime=400;
int dispMode=0;
int demoModeFlag=0;
int demoTime=140;
int demoCounter=0;
unsigned long demoCountUp=0;
unsigned long chanCursor=0;

// analog in = analog 4
//digital 5,6,7

// modes : default (lines)
//         - chan select
//         - wide
//         - dual chan

int offsetCounter=0;
int offsetTime=470;

int offsetDir=0;

int statusFlag=0;
int status2Flag=0;
int statusCenterFlag=0;
int dataPin=2;
int queueLen=24;


//int iteration=0;
//int channel=1;
unsigned long displayHigh=13;
unsigned long displayLow=13;
unsigned long bit12=1<<12;
unsigned long displayLed=0;
//unsigned long mask=1;
unsigned long temp=0;

int offset=0;

int midiMessageType;
int midiStatus;
int midiParam1;
int midiParam2;
int MESSAGENoteOn=0x90;   
int MESSAGENoteOff=0x80;  
int MESSAGEPitch=0xE0;
int MESSAGECC=0xB0;
int MESSAGESys=0xF0;  // wat?
int MESSAGEStatus=0x80;


int chanMatch(int chan)
{
  if ((1<<chan)&channelMask) return(1); else return(0);
}

void handleNoteOn(int chan, int note, int vel)
{
            //check chan
            if (chanMatch(chan&0x0F))
            if (dispMode==0)
            notesL[note%12]=fullBright; else
            {
              if ((note%24)<12) notesL[note%12]=startFade; else
                                notesH[note%12]=startFade;
                                blinkCenter();
            }
            demoCountUp=0;
}

void allFade()
{
           for (int i=0;i<12;i++)
           { notesL[i]=startFade;
             notesH[i]=startFade;
           }
}

void handleNoteOff(int chan, int note, int vel)
{
           if (chanMatch(chan&0x0F))
           if (dispMode==0)
            notesL[note%12]=startFade; else
            {
              if ((note%24)<12) notesL[note%12]=startFade; else
                                notesH[note%12]=startFade;
            }
}


//int tempTresh=10;

void drawShade()
{
            if (PWMcounter>PWMlen) PWMcounter=0;
          
            displayLow=0;
            displayHigh=0;
            int tempBit=1;
            for (int i=0;i<12;i++)
             {  
              if (PWMcounter<fadeValues[notesL[i]]) {
                   displayLow=displayLow | tempBit;  }
              if (PWMcounter<fadeValues[notesH[i]]) {
                   displayHigh=displayHigh | tempBit;  }
             
               tempBit=tempBit<<1;
             }
             if (dispMode==0)
                displayHigh=displayLow; // if dispmode 0 we are displaying same one octave on both displays
             PWMcounter++;  
}


int midiReadCounter=0;
void readMidi()
{
   while (Serial.available())
       {
          midiRead=Serial.read();
         // Serial.write(midiRead);  // midi through
        
           if (midiRead & MESSAGEStatus)  // status byte
              {
                 midiStatus=midiRead;  
                midiReadCounter=1; 
               
              } else
             {  // begin data byte
               if (midiReadCounter==1) 
                     {
                        midiReadCounter=2;         
                        midiParam1=midiRead;   
                      
                     } else
               if (midiReadCounter==2) 
                     {
                       midiReadCounter=1;
                       midiParam2=midiRead;  
                       handleMidiMessage(midiStatus,midiParam1,midiParam2);          // to powinno byc po trzecim bajcie !
                  
                 if (((midiStatus&0xF0)==0x90)&&(midiParam2!=0)) // selective through - midi note ON only
                 {
                           Serial.write(midiStatus);
                           Serial.write(midiParam1);
                           Serial.write(midiParam2);
                 }
                     if (midiStatus==0xBE) // selective through - midi note ON only
                 {
                           Serial.write(0x9E);
                           Serial.write(midiParam1);
                           Serial.write(midiParam2);
                   
                 }
          
           } else             
                      {    midiReadCounter=0; }
                      
             }   // end data byte
            
       } // end serial avaliable
}
void handleMidiMessage(int midiStatus,int midiParam1,int midiParam2)
{
    if ((midiStatus&0xF0)==0x90)
            {
            
            if (midiParam2>0)
                    handleNoteOn(midiStatus&0x0F,midiParam1,midiParam2); 
               else 
                    handleNoteOff(midiStatus&0x0F,midiParam1,midiParam2); 
//                   blink2();  
            }
            
          if ((midiStatus&0xF0)==0x80)
            {
                   handleNoteOff(midiStatus&0x0F,midiParam1,midiParam2); 
            }
         
}




void setup() { 
                 Serial.begin(31250);                // MIDI Baud
                 pinMode(clockPin,OUTPUT);
                 pinMode(strobePin,OUTPUT);
                 pinMode(statusPin,OUTPUT);
                 pinMode(status2Pin,OUTPUT);
                 pinMode(statusCenterPin,OUTPUT);
                 pinMode(dataPin,OUTPUT);
                 pinMode(button1,INPUT);
                 pinMode(button2,INPUT); 
                 pinMode(button3,INPUT);
                 demoCounter=500;
                 demoMode();
}


void fadeNotes(){   
              int noteFadeTime;
              if (dispMode==0) noteFadeTime=30; else noteFadeTime=150;
               if ((millis()-lastMsFade)>noteFadeTime) { // fade notes //NOTE FADE TIME IN MODE2 
                     lastMsFade=millis();
                     for (int i=0;i<12;i++)
                     {
                       if ((notesL[i]<fullBright)&&(notesL[i]>0)) notesL[i]--;
                       if ((notesH[i]<fullBright)&&(notesH[i]>0)) notesH[i]--;
                       
                     }
               }
}

void addRandomNotes()
{
        if ((millis()-lastRandomNoteMs)>300) { // random notes add
               lastRandomNoteMs=millis();
      //         int rand=random(11);
               notesL[random(12)]=fullBright-1;
               
               }
  
}


void drawChans()
{
                blinkCounter++;
                if (blinkCounter>blinkTime) blinkCounter=0; 
              
              // read the knob and update the cursor begin
               currentAnalogRead=analogRead(knob);
              if (abs(lastAnalogRead-currentAnalogRead)>3)
              {
                chanCursor=map(currentAnalogRead,0,1024,0,24);  // read the knob for cursor position
                lastAnalogRead=currentAnalogRead;
                if (channelMask&(1<<chanCursor)) blinkCounter=0; else blinkCounter=19*blinkTime/20; // dirty but works - sets the cursor
              }// read the knob and update the cursor end 
                
                displayLed=0; // clear display
                for (int i=0;i<18;i++)
                     if (channelMask&(1<<i)) displayLed=displayLed|(1<<i); // display the mask
                 
                
                if ((channelMask&(1<<chanCursor))&&(blinkCounter>blinkTime/6) ||    // dual experssion to make the blink different
                    ~(channelMask&(1<<chanCursor))&&(blinkCounter>19*blinkTime/20))     // depending on the value below it
                     displayLed=displayLed | (1<<chanCursor); else
                     displayLed=displayLed & ~(1<<chanCursor); 
                
                // output to display
                for (int i=0; i<queueLen;i++)
                    {
                         digitalWrite(dataPin,((displayLed>>(queueLen-i-1))&1));
                         clockTick();
                     } 
               strobeTick();
 
}

int demoMode()
{
                notesL[0]=0;
                notesL[1]=1;
                notesL[2]=2;
                notesL[3]=3;
                notesL[4]=4;
                notesL[5]=5;
                notesL[6]=6;
                notesL[7]=7;
                notesL[8]=8;
                notesL[9]=9;
                notesL[10]=10;
                notesL[11]=10;
               // demoCounter=1000;
              while ((Serial.available()==0)||(demoCounter>0)) // or demo mode? ||
              {  offsetCounter++;
                 if (offsetCounter>demoTime) {
                                            offsetCounter=0;
                                            offset++; 
                                            if (offset>11) offset=0;
                                           }
                if (Serial.available()!=0) demoCounter=0; // nie wiem czy to dobrze
                 drawShade();
                 updateDisplay();
               //  if (digitalRead(button3)==HIGH) { demoModeFlag=0; delay(50); }
               if (demoCounter>0) demoCounter--;
               //readMidi();
              }
              offset=0;
              return(0);
  
}

void loop()
{
                 if (globalMode==0) {   
                      readMidi();            
                      buttonsRead();
                      drawShade();
                      updateOffsets();
                      updateDisplay();
                      readMidi();
                      fadeNotes();
                      demoCountUp++;
 
                      if (demoCountUp>100000)
                          {
                                demoCounter=3000;
                                demoMode();
                                demoCountUp=0;
                          }
                       }
                  if (globalMode==-1) {    // channel selection mode
                     readMidi();
                     drawChans();
                     buttonsRead();
                  } 
                     updateLeds();
}
