The External Slicer testing was performed on July 13, 2005 (with details below):

*********************************************
README FOR EXTERNAL SLICER:
*********************************************

**SOFTWARE MODIFICATION**

1) In APP_LCDTV\FW\System\System.h enable the macro define for "PAL_TV_SYSTEM"
2) In APP_LCDTV\FW\System\System.h enable the macro define for "EXTERNAL_SLICER"
3) In the project file APP_LCDTV\FW\App_LCDTV.ide add the following 4 nodes to the StdLibReplace folder:
(A) ExternalSlicer.c
(B) flagline_int.c 
(C) gm_GetNextPacket.c 
(D) gm_VBIInit.c). 

**HARDWARE MODIFICATION**

1) Place a jumper on JP501 - pins 5,6 (closed), such that HUDSON is set to IROM.
2) Ensure that the dip switches on the RD2 board are set to the ON settings (S401, S402, S403, S404).

**TEST APPARATUS**

- Tested on an RD2 platform with Cortez BE silicon using the VPX3238F external decoder
- The VPX decoder was mounted on the Micronas application board with a CVBS input test input on the VIN1 port

**********************************************
Troubleshooting & Notes:
**********************************************

Step 1:

Ensure that after integration of all the necessary files, download the code and enable both the main and pip 
to show a valid signal (any input is fine). Run Appstest #209 and make sure that the input to the main and 
pip are now coming from the VPX. Ensure that a valid image is seen. If no valid image is seen, most likely 
the digital input port’s clock is not routed correctly (perhaps due to different connections on the TTE setup 
versus Cortez RD2). Experiment with the register “CLOCK_CONFIG2” in GProbe. Specifically try 
changing the bits “IMP_CLK_SEL” and “IPP_CLK_SEL” such that it chooses the valid input clock from 
the digital port (there are 4 operational clocks – try changing the bit values from 0-3). We used the 
VPX3228F for development with the application board and a CVBS input connected to the Vin1 port.

Only until a valid display is shown, should Teletext be tested, as the capturing of the data will not be 
possible if the input clock is routed incorrectly to Cortez.

Step 2:

If the register “CLOCK_CONFIG2” required a modification in step 1 above, ensure that the updated value 
is copied into the function EnableExternalSlicer on line #105 of <<ExternalSlicer.c>>

NOTES:

The external slicer setup only occurs after the “TEXT” button is pressed on the remote. This is different 
from the internal slicer solution where in the background Teletext packets are constantly being parsed and 
sent to the Teletext database. As a result, the Teletext display will require more time to display valid data. 
You will find initially that as the Teletext database is being populated, there will be some errors found on 
some pages.

We also noticed in our testing with the <<VG2.EPR>> file with Teletext 2.5, some pages consistently had 
problems when displayed. Specifically pages 192, and 257, and 711 were most troublesome. This will be 
looked at in detail next week. Keep in mind the external slicer solution is constantly outputting data into the 
VBI circular buffer (i.e. even non-ttx packets are captured and placed in the buffer – unlike the internal 
slicer solution). As a result, the parsing engine is constantly busy trying to filter out only the valid packets. 
Since this logic is done in software as opposed to HW, there is definitely a performance tradeoff. There is 
another performance hit since the data size is doubled with the VPX (sending data out only on the luma 
instead of placing it internally in a FIFO and sending the data continuously).

>> Updated on September 19, 2005
Regarding pages 192, 257 and 711 corruption on test file VG2.EPR, it was found 
that the current VPX/Cortez configuration is not able to get VBI line6 data 
whereas the above pages were using line 6 for header packets.  As a result of 
missing packet 0, pages transmitted adjacently were mixed up.  A modifed 
version of VG2.EPR (named VG2_.EPR) which does not use VBI line6 for TTX was 
used to re-run the test, the issue on page 192 and 257 were eliminated; 
however, page 711 still missing line but that could be due the nature of that 
page which supoose to 'crack' the slicer clock.

