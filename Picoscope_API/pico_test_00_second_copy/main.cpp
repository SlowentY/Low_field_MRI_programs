//2023.11.10 1st picoscope test
//2023.11.14 add set channel function
//2023.11.17 + led blink + functions for capturing data, save buffer data to file
//2023.11.17 + level trigger
//2023.11.29 +  DataIsReady block
//2023.12.13 adopted for program for noise measurements

#include <stdio.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <fstream>


/* Headers for Windows */
#ifdef _WIN32
#include "windows.h"
#include <conio.h>
//#include "libps4000a-1.0/ps4000aApi.h"
#include "inc/ps4000aApi.h"

#include "libps4000a-1.0/ps4000aApi.h"

#ifndef PICO_STATUS
#include <libps4000a-1.0/PicoStatus.h>
#endif

//#include <ps4000aApi.h>
//#include <cinttypes>
//#include <C:\\Program Files\\CodeBlocks\\MinGW\\lib\\gcc\\x86_64-w64-mingw32\\8.1.0\\include\\c++\\cstdint>

#else
#include <sys/types.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>


#include <libps4000a-1.0/ps4000aApi.h>
#ifndef PICO_STATUS
#include <libps4000a-1.0/PicoStatus.h>
#endif

#define Sleep(a) usleep(1000*a)
#define scanf_s scanf
#define fscanf_s fscanf
#define memcpy_s(a,b,c,d) memcpy(a,c,d)

typedef enum enBOOL{FALSE,TRUE} BOOL;



/* A function to detect a keyboard press on Linux */
int32_t _getch()
{
        struct termios oldt, newt;
        int32_t ch;
        int32_t bytesWaiting;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~( ICANON | ECHO );
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        setbuf(stdin, NULL);
        do {
                ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
                if (bytesWaiting)
                        getchar();
        } while (bytesWaiting);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
}

int32_t _kbhit()
{
        struct termios oldt, newt;
        int32_t bytesWaiting;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~( ICANON | ECHO );
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        setbuf(stdin, NULL);
        ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return bytesWaiting;
}

int32_t fopen_s(FILE ** a, const char * b, const char * c)
{
FILE * fp = fopen(b,c);
*a = fp;
return (fp>0)?0:-1;
}
typedef void (*ps4000aBlockReady)
(
int16_t handle,
PICO_STATUS status,
void * pParameter
)
/* A function to get a single character on Linux */
#define max(a,b) ((a) > (b) ? a : b)
#define min(a,b) ((a) < (b) ? a : b)
#endif

using namespace std;

string return_fun(uint32_t value)
{
string rv;
switch (value)
{
//return values from set channel
case PICO_OK:                                        rv  = "PICO_OK"; break;
case PICO_USER_CALLBACK:                             rv = "PICO_USER_CALLBACK" ; break;
case PICO_INVALID_HANDLE:                            rv = "INVALID HANDLE"; break;
case PICO_INVALID_CHANNEL:                           rv = "PICO_INVALID_CHANNEL"; break;
case PICO_INVALID_VOLTAGE_RANGE:                     rv = "PICO_INVALID_VOLTAGE_RANGE"; break;
case PICO_DRIVER_FUNCTION:                           rv = "PICO_DRIVER_FUNCTION"; break;
case PICO_INVALID_COUPLING:                          rv =  "PICO_INVALID_COUPLING"; break;
case PICO_INVALID_ANALOGUE_OFFSET:                   rv = "PICO_INVALID_ANALOG_OFFSET"; break;
case PICO_WARNING_PROBE_CHANNEL_OUT_OF_SYNC:         rv = "PICO_WARNING_PROBE_CHANNEL_OUT_OF_SYNC"; break;
case PICO_PROBE_NOT_POWERED_WITH_DC_POWER_SUPPLY:    rv = "PICO_PROBE_NOT_POWERED_WITH_DC_POWER_SUPPLY"; break;
case PICO_PROBE_POWER_DC_POWER_SUPPLY_REQUIRED:      rv = "PICO_PROBE_POWER_DC_POWER_SUPPLY_REQUIRED"; break;
//return values from flash led
case PICO_BUSY: rv = "PICO_BUSY"; break;
case PICO_MEMORY: rv = "PICO_MEMORY"; break;
case PICO_INTERNAL_ERROR: rv = "PICO_INTERNAL_ERROR"; break;
case PICO_POWER_SUPPLY_UNDERVOLTAGE: rv = "PICO_POWER_SUPPLY_UNDERVOLTAGE"; break;
case PICO_NOT_RESPONDING: rv = "PICO_NOT_RESPONDING"; break;
case PICO_POWER_SUPPLY_CONNECTED: rv = "PICO_POWER_SUPPLY_CONNECTED"; break;
case PICO_POWER_SUPPLY_NOT_CONNECTED: rv = "PICO_POWER_SUPPLY_NOT_CONNECTED"; break;
case PICO_TIMEOUT: rv = "PICO_TIMEOUT"; break;
case PICO_RESOURCE_ERROR: rv = "PICO_RESOURCE_ERROR"; break;
case PICO_DEVICE_NOT_FUNCTIONING: rv = "PICO_DEVICE_NOT_FUNCTIONING"; break;
// return values from open
case PICO_OS_NOT_SUPPORTED:              rv = "PICO_OS_NOT_SUPPORTED" ; break;
case PICO_OPEN_OPERATION_IN_PROGRESS:    rv = "PICO_OPEN_OPERATION_IN_PROGRESS"; break;
case PICO_EEPROM_CORRUPT:                rv = "PICO_EEPROM_CORRUPT"; break;
case PICO_KERNEL_DRIVER_TOO_OLD:         rv = "PICO_KERNEL_DRIVER_TOO_OLD"; break;
case PICO_FW_FAIL:                       rv = "PICO_FW_FAIL"; break;
case PICO_MAX_UNITS_OPENED:              rv = "PICO_MAX_UNITS_OPENED"; break;
case PICO_NOT_FOUND:                     rv = "PICO_NOT_FOUND"; break;
case PICO_USB3_0_DEVICE_NON_USB3_0_PORT: rv = "PICO_USB3_0_DEVICE_NON_USB3_0_PORT"; break;
case PICO_MEMORY_FAIL:                   rv = "PICO_MEMORY_FAIL"; break;
case PICO_HARDWARE_VERSION_NOT_SUPPORTED:rv = "PICO_HARDWARE_VERSION_NOT_SUPPORTED"; break;
case PICO_NOT_USED:                      rv = "PICO_NOT_USED"; break;
case PICO_FPGA_FAIL:                     rv = "PICO_FPGA_FAIL"; break;

//return values from get timebase
case PICO_TOO_MANY_SAMPLES:    rv = "PICO_TOO_MANY_SAMPLES"; break;
case PICO_INVALID_TIMEBASE:    rv = "PICO_INVALID_TIMEBASE"; break;
case PICO_INVALID_PARAMETER:    rv = "PICO_INVALID_PARAMETER"; break;
case PICO_SEGMENT_OUT_OF_RANGE:    rv = "PICO_SEGMENT_OUT_OF_RANGE"; break;
//return values from run block
case PICO_INVALID_TRIGGER_CHANNEL:    rv = "PICO_INVALID_TRIGGER_CHANNEL"; break;
case PICO_INVALID_CONDITION_CHANNEL:    rv = "PICO_INVALID_CONDITION_CHANNEL"; break;
case PICO_CONFIG_FAIL:    rv = "PICO_CONFIG_FAIL"; break;
case PICO_TRIGGER_ERROR:    rv = "PICO_TRIGGER_ERROR"; break;
case PICO_NOT_USED_IN_THIS_CAPTURE_MODE:    rv = "PICO_NOT_USED_IN_THIS_CAPTURE_MODE"; break;
case PICO_TRIGGER_WITHIN_PRE_NOT_ALLOWED_WITH_DELAY:    rv = "PICO_TRIGGER_WITHIN_PRE_NOT_ALLOWED_WITH_DELAY"; break;
case PICO_INVALID_NUMBER_CHANNELS_FOR_RESOLUTION:    rv = "PICO_INVALID_NUMBER_CHANNELS_FOR_RESOLUTION"; break;
case PICO_NOT_ENOUGH_SEGMENTS:    rv = "PICO_NOT_ENOUGH_SEGMENTS"; break;
case PICO_NO_TRIGGER_ENABLED_FOR_TRIGGER_IN_PRE_TRIG:    rv = "PICO_NO_TRIGGER_ENABLED_FOR_TRIGGER_IN_PRE_TRIG"; break;
//return values from set data buffer
case PICO_RATIO_MODE_NOT_SUPPORTED:    rv = "PICO_RATIO_MODE_NOT_SUPPORTED"; break;
//return values from get values
case PICO_NO_SAMPLES_AVAILABLE:    rv = "PICO_NO_SAMPLES_AVAILABLE"; break;
case PICO_DEVICE_SAMPLING:    rv = "PICO_DEVICE_SAMPLING"; break;
case PICO_NULL_PARAMETER:    rv = "PICO_NULL_PARAMETER"; break;
case PICO_DATA_NOT_AVAILABLE:    rv = "PICO_DATA_NOT_AVAILABLE"; break;
case PICO_STARTINDEX_INVALID:    rv = "PICO_STARTINDEX_INVALID"; break;
case PICO_INVALID_SAMPLERATIO:    rv = "PICO_INVALID_SAMPLERATIO"; break;
case PICO_INVALID_CALL:    rv = "PICO_INVALID_CALL"; break;
case PICO_BUFFERS_NOT_SET:    rv = "PICO_BUFFERS_NOT_SET"; break;
case PICO_ETS_NOT_RUNNING:    rv = "PICO_ETS_NOT_RUNNING"; break;
//return value from SetTriggerChannelConditions
case PICO_TOO_MANY_CHANNELS_IN_USE :    rv = "PICO_TOO_MANY_CHANNELS_IN_USE"; break;
case PICO_INVALID_CONDITION_INFO:    rv = "PICO_INVALID_CONDITION_INFO"; break;
case PICO_DUPLICATE_CONDITION_SOURCE:    rv = "PICO_DUPLICATE_CONDITION_SOURCE"; break;


case PICO_CANCELLED: rv = "PICO_CANCELLED"; break;

default:
                                                     rv = "unknown return value = " +  to_string(value);

} //switch
return rv;
} //func



int main()
{
int16_t handle;
uint32_t retval_open{0};
uint32_t r{0};
string rs;
cout << endl;
cout << "handle = " << handle << endl << endl;

cout << "////////////////open pico//////////////"<< endl<< endl;
retval_open = ps4000aOpenUnit(&handle, NULL);
rs = return_fun(retval_open);
cout << endl << "rs = "<< rs << endl;

PS4000A_CHANNEL test_channel {PS4000A_CHANNEL_A} ;
//PS4000A_CHANNEL noise2_channel {PS4000A_CHANNEL_B};
//PS4000A_CHANNEL noise3_channel {PS4000A_CHANNEL_C};
//PS4000A_CHANNEL noise4_channel {PS4000A_CHANNEL_D};
//PS4000A_CHANNEL amp_channel {PS4000A_CHANNEL_E};
//PS4000A_CHANNEL trig_channel {PS4000A_CHANNEL_F};
int16_t enabled {true};
PS4000A_COUPLING type_AC  {PS4000A_AC};
//PS4000A_COUPLING type_DC {PS4000A_DC};

// set Voltage ranges

/*
0  PICO_X1_PROBE_10MV ±10 mV
1 PICO_X1_PROBE_20MV ±20 mV
2 PICO_X1_PROBE_50MV ±50 mV
3 PICO_X1_PROBE_100MV ±100 mV
4 PICO_X1_PROBE_200MV ±200 mV
5 PICO_X1_PROBE_500MV ±500 mV
6 PICO_X1_PROBE_1V ±1 V
7 PICO_X1_PROBE_2V ±2 V
8 PICO_X1_PROBE_5V ±5 V
9 PICO_X1_PROBE_10V ±10 V
10 PICO_X1_PROBE_20V ±20 V
11 PICO_X1_PROBE_50V ±50 V
12 PICO_X1_PROBE_100V ±100 V
13 PICO_X1_PROBE_200V ±200 V
*/
PICO_CONNECT_PROBE_RANGE test_range {PICO_X1_PROBE_500MV};
//PICO_CONNECT_PROBE_RANGE amp_range {PICO_X1_PROBE_10V};
//PICO_CONNECT_PROBE_RANGE trig_range {PICO_X1_PROBE_10V};
float analogOffset  {0};




/*
cout << "////////////////Register your probe interaction//////////////"<< endl<< endl;
ps4000aProbeInteractions callback;

r =  ps4000aSetProbeInteractionCallback(handle, callback);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;
*/





int16_t start{10};
r = ps4000aFlashLed(handle, start);
rs = return_fun(r);
cout << endl << rs << endl;



//ps4000aGetTimebase() – find out what timebases are available
cout << "////////////////get timebase pico//////////////"<< endl<< endl;
uint32_t timebase {1};// sampling frequency = 50MHz/(timebase-2) sampling interval = 20ns*(timebase-2)
int32_t noSamples{102400};
int32_t * timeIntervalNanoseconds{nullptr};
int32_t * maxSamples;
uint32_t segmentIndex{0};
r = ps4000aGetTimebase( handle, timebase,  noSamples,  nullptr, nullptr, segmentIndex);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;


//ps4000aSetTriggerChannelConditions() – specify which channels to trigger on
cout << "////////////////ps4000aSetTriggerChannelConditions()//////////////"<< endl<< endl;

PS4000A_CONDITION conditions[1];
conditions[0].source = test_channel;
conditions[0].condition = PS4000A_CONDITION_TRUE;
uint16_t nConditions {1};
PS4000A_CONDITIONS_INFO info {PS4000A_CLEAR};
r = ps4000aSetTriggerChannelConditions( handle, conditions,  nConditions,  info);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;

cout << "////////////////set channel pico//////////////"<< endl<< endl;
r = ps4000aSetChannel(handle, test_channel, enabled, type_AC, test_range, analogOffset);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;

//r = ps4000aSetChannel(handle, noise2_channel, enabled, type_AC, noise_range, analogOffset);
//rs = return_fun(r);
//cout << endl << "rs = "<< rs << endl;
//
//r = ps4000aSetChannel(handle, noise3_channel, enabled, type_AC, noise_range, analogOffset);
//rs = return_fun(r);
//cout << endl << "rs = "<< rs << endl;
//
//r = ps4000aSetChannel(handle, noise4_channel, enabled, type_AC, noise_range, analogOffset);
//rs = return_fun(r);
//cout << endl << "rs = "<< rs << endl;
//
//r = ps4000aSetChannel(handle, amp_channel, enabled, type_AC, amp_range, analogOffset);
//rs = return_fun(r);
//cout << endl << "rs = "<< rs << endl;
//
//r = ps4000aSetChannel(handle, trig_channel, enabled, type_DC, trig_range, analogOffset);
//rs = return_fun(r);
//cout << endl << "rs = "<< rs << endl;

//ps4000aSetTriggerChannelDirections() – set up signal polarities for triggering
cout << "////////////////ps4000aSetTriggerChannelDirections()//////////////"<< endl<< endl;
PS4000A_DIRECTION directions[1];
directions[0].channel = test_channel;
directions[0].direction = PS4000A_ABOVE;
int16_t nDirections = 1;
r = ps4000aSetTriggerChannelDirections( handle, directions, nDirections);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;


//ps4000aSetTriggerChannelProperties() – set up trigger thresholds
cout << "////////////////ps4000aSetTriggerChannelProperties()//////////////"<< endl<< endl;

PS4000A_TRIGGER_CHANNEL_PROPERTIES  channel_properties [1];
channel_properties[0].channel = test_channel;
channel_properties[0].thresholdUpper = 16381 ; //Threshold voltage in 16 bit scale from set voltage on channel
channel_properties[0].thresholdUpperHysteresis = 4095;
channel_properties[0].thresholdMode = PS4000A_LEVEL;

int16_t nChannelProperties {1};
int16_t auxOutputEnable ;
int32_t autoTriggerMilliseconds {10000};

r = ps4000aSetTriggerChannelProperties( handle, channel_properties, nChannelProperties, auxOutputEnable, autoTriggerMilliseconds);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;

//ps4000aSetTriggerDelay() – set up post-trigger delay
cout << "////////////////ps4000aSetTriggerDelay()//////////////"<< endl<< endl;
uint32_t delay {0};
r =  ps4000aSetTriggerDelay( handle, delay);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;


//ps4000aRunBlock() – start block mode
cout << "////////////////ps4000aRunBlock()//////////////"<< endl<< endl;
int32_t noOfPreTriggerSamples {0};
int32_t noOfPostTriggerSamples {102400};
int32_t * timeIndisposedMs;
ps4000aBlockReady lpReady;
void * pParameter;

r = ps4000aRunBlock(handle,noOfPreTriggerSamples, noOfPostTriggerSamples,  timebase, nullptr, segmentIndex , nullptr, nullptr);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;

cout << "////////////////ps4000isReady()//////////////"<< endl<< endl;//check if data is ready

int16_t ready{0};
while (ready==0)
{
cout << "Data NOT ready"<<endl;
r = ps4000aIsReady(handle, &ready);
}



//ps4000aSetDataBuffer() – register data buffer with driver
cout << "////////////////SetDataBuffer//////////////"<< endl<< endl;
int32_t bufferLth{102400};

int16_t buffer1[bufferLth];
//int16_t buffer2[bufferLth];
//int16_t buffer3[bufferLth];
//int16_t buffer4[bufferLth];
//int16_t buffer5[bufferLth];
//int16_t buffer6[bufferLth];

//uint32_t segmentIndex;
PS4000A_RATIO_MODE mode {PS4000A_RATIO_MODE_NONE};
r = ps4000aSetDataBuffer(handle, test_channel, buffer1, bufferLth, segmentIndex, mode);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;
//
////uint32_t segmentIndex;
//r = ps4000aSetDataBuffer(handle, noise2_channel, buffer2, bufferLth, segmentIndex, mode);
//rs = return_fun(r);
//cout << endl << "rs = "<< rs << endl;
//
////uint32_t segmentIndex;
//r = ps4000aSetDataBuffer(handle, noise3_channel, buffer3, bufferLth, segmentIndex, mode);
//rs = return_fun(r);
//cout << endl << "rs = "<< rs << endl;
//
////uint32_t segmentIndex;
//r = ps4000aSetDataBuffer(handle, noise4_channel, buffer4, bufferLth, segmentIndex, mode);
//rs = return_fun(r);
//cout << endl << "rs = "<< rs << endl;
//
////uint32_t segmentIndex;
//r = ps4000aSetDataBuffer(handle, amp_channel, buffer5, bufferLth, segmentIndex, mode);
//rs = return_fun(r);
//cout << endl << "rs = "<< rs << endl;
//
////uint32_t segmentIndex;
//r = ps4000aSetDataBuffer(handle, trig_channel, buffer6, bufferLth, segmentIndex, mode);
//rs = return_fun(r);
//cout << endl << "rs = "<< rs << endl;

//this_thread::sleep_for(1000ms);


// ps4000aGetValues() – retrieve block-mode data
cout << "////////////////GetValues//////////////"<< endl<< endl;
uint32_t startIndex{0};
uint32_t  noOfSamples {102400};
uint32_t downSampleRatio {1};
PS4000A_RATIO_MODE downSampleRatioMode{PS4000A_RATIO_MODE_NONE};
//uint32_t segmentIndex;
int16_t  overflow;
r = ps4000aGetValues(handle, startIndex, &noOfSamples, downSampleRatio, downSampleRatioMode,  segmentIndex, &overflow);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;
//ps4000aStop() – stop data capture
cout << "////////////////Pico Stop//////////////"<< endl<< endl;
r = ps4000aStop(handle);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;


ps4000aCloseUnit(handle);
cout << "////////////////PICO CLOSED//////////////"<< endl<< endl;


cout << endl;
cout << "handle = " << handle << endl;
cout << endl;


cout << "////////////////Write data in text file//////////////"<< endl<< endl;

ofstream testfile ("test_pico.csv");
  if (testfile.is_open())
//    testfile << "noise_1_channel" << ","<< "noise_2_channel"<< ","<< "noise_3_channel"<< ","<< "noise_4_channel"<< ","<< "amp_channel"<< ","<< "trig_channel"<<endl;
    testfile << "test_channel" <<endl;

  {
    for (int i = 0;i <noOfSamples; i++)
    {
//    testfile <<  buffer1[i]<<","<<buffer2[i]<<","<<buffer3[i]<<","<<buffer4[i]<<","<<buffer5[i]<<","<<buffer6[i]<<endl;
    testfile <<  buffer1[i]<< endl;

    }//end for

    testfile.close();
  }

    return 0;
} //int main end

