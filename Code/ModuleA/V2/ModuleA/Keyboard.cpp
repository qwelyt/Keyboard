#include <HID.h>
#include <string.h>
#include "keyboard.h"

const uint8_t REPORT_ID = 0x02;

static const uint8_t hidReportDescriptor[] PROGMEM = {

  //  Keyboard
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 47
  0x09, 0x06,                    // USAGE (Keyboard)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x85, REPORT_ID,               //   REPORT_ID (2)
  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)

  0x19, 0xe0,                      //   USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
  0x75, 0x01,                    //   REPORT_SIZE (1)

  0x95, 0x08,                      //   REPORT_COUNT (8)
  0x81, 0x02,                    //   INPUT (Data,Var,Abs)
  0x95, 0x01,                    //   REPORT_COUNT (1)
  0x75, 0x08,                    //   REPORT_SIZE (8)
  0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)

  0x95, 0x06,                      //   REPORT_COUNT (6)
  0x75, 0x08,                    //   REPORT_SIZE (8)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)

  0x19, 0x00,                      //   USAGE_MINIMUM (Reserved (no event indicated))
  0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
  0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
  0xc0,                          // END_COLLECTION
};

typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;

void initKeyboard() {
  static HIDSubDescriptor node(hidReportDescriptor, sizeof(hidReportDescriptor));
  HID().AppendDescriptor(&node);
}

void sendKeyBuffer(uint8_t meta, uint8_t keys[]) {
  KeyReport report;

  report.modifiers = meta;
  
  for(byte b=0; b<6; ++b){
    report.keys[b] = keys[b];
  }

//  Serial.println();
//  Serial.print(report.modifiers);
//  for(byte b=0; b<6; ++b){
//    Serial.print(" ");
//    Serial.print(report.keys[b]);
//  }
//  Serial.println();

  HID().SendReport(REPORT_ID, &report, sizeof(report));
}
