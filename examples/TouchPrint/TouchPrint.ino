#include "gt911_lite.h"
#include "Wire.h"

#define TOUCH_SDA  39
#define TOUCH_SCL  40
#define TOUCH_INT  3
#define TOUCH_RST  26

GT911_Lite tp;

TwoWire wire(0);

void setup() {
  wire.begin(TOUCH_SDA, TOUCH_SCL);
  Serial.begin(115200);
  Serial.println("GT911 Lite Example: Ready");
  tp.begin(&wire);
}

void loop() {
  if (tp.read()) {
    for (int i = 0; i < tp.touches; i++) {
      Serial.print("Touch "); Serial.print(i + 1); Serial.print(": ");
      Serial.print("  x: ");    Serial.print(tp.points[i].x);
      Serial.print("  y: ");    Serial.print(tp.points[i].y);
      Serial.print("  size: "); Serial.println(tp.points[i].size);
    }
  }
}
