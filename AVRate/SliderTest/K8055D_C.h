/************************************************************
*************************************************************
** Microsoft Visual C++ Project for the K8055 USB I/O Card **
**                                                         **
**                  Copyright Velleman 2005                **
**                      www.Velleman.be                    **
**                                                         **
**                                                         **
**                                                         **
**                       Developed by                      **
**                      RE-Applications                    **
**                  www.RE-Applications.be                 **
*************************************************************
*************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#define FUNCTION __declspec(dllexport)

FUNCTION long __stdcall OpenDevice(long CardAddress);
FUNCTION void __stdcall CloseDevice();
FUNCTION long __stdcall ReadAnalogChannel(long Channel);
FUNCTION void __stdcall ReadAllAnalog(long *Data1, long *Data2);
FUNCTION void __stdcall OutputAnalogChannel(long Channel, long Data);
FUNCTION void __stdcall OutputAllAnalog(long Data1, long Data2);
FUNCTION void __stdcall ClearAnalogChannel(long Channel); 
FUNCTION void __stdcall ClearAllAnalog();
FUNCTION void __stdcall SetAnalogChannel(long Channel); 
FUNCTION void __stdcall SetAllAnalog();
FUNCTION void __stdcall WriteAllDigital(long Data);
FUNCTION void __stdcall ClearDigitalChannel(long Channel);
FUNCTION void __stdcall ClearAllDigital();
FUNCTION void __stdcall SetDigitalChannel(long Channel);
FUNCTION void __stdcall SetAllDigital();
FUNCTION bool __stdcall ReadDigitalChannel(long Channel);
FUNCTION long __stdcall ReadAllDigital();
FUNCTION long __stdcall ReadCounter(long CounterNr);
FUNCTION void __stdcall ResetCounter(long CounterNr);
FUNCTION void __stdcall SetCounterDebounceTime(long CounterNr, long DebounceTime);

FUNCTION void __stdcall Version();
FUNCTION long __stdcall SearchDevices();
FUNCTION long __stdcall SetCurrentDevice(long lngCardAddress);

#ifdef __cplusplus
}
#endif



