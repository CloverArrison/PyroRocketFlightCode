#ifndef FLASH_H
#define FLASH_H

#include <SD.h>
#include <SPI.h>
#include "DATA.h"

class myFlash{
public:
  myFlash(int cs);                  // Constructor takes in the chip select from config
  void flashSetup(String SDname);   // Starts flash chip, uses name for printing
  void printToSerial() ;            // Prints to serial what would be printed to file
  void handleWriteFlash();          // Used by main to write data each loop, handles file name
private:
  bool makeFile(String fname);      // Makes a new file if one with the same name doesn't exist 
  void delFile(String fname);       // Deletes a file with a specified name
  void writeDataFile(File file);    // Writes formatted data to a file
};

#endif