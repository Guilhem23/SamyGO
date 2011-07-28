// autobright.h
//
// common definitions for 
// LCD backlight auto adjusting in accordance with ambient light level
// for Samsung TV with T-RBYDEUC firmware and Local dimming LED backlight
// (models LE..A956 and LE..A756)
// and external light sensor connected to USB or Ex-link
//
///////////////////////////////////////////////////////////////////////////////

typedef struct {
  float x;
  float y;
} aproxTable_t;

#define MAX_SENSOR_CONV 7

//extern aproxTable_t photoSensorConv[MAX_SENSOR_CONV];

#define CONF_PATH "/mtd_rwarea/autobright.conf"

typedef void (*sensorCallBack_t) (unsigned light);

