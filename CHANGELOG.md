Change Log v 1.1:
Bug fixes:

·	Point data for files over 2 GB was not read correctly due to limitations of <int> data type. It is changed to <__int64> which can handle the data > 2 GB. Now the node can easily read files over 2 GB.

Notes
·	The same changes needs to be applied for KP_PointCacheReader for this bugfix. I am not aware if Kai has fixed this already in other updates to his code. But the one I had from rray.de does not have this change.

·	The code for this bugfix is not supported on Linux. However, in case you want to compile for Linux, please note that the <__int64> data type for windows translates to <long long> on a gcc compiler for Linux. I will add support for Linux once I have a machine with Linux up and running. Till then you have to change the code yourself.


·	Suppressed unnecessary warning messages in cases of pc2 file not specified and unable to open file. They were kind of annoying. Just uncomment my code lines to bring them back if you want.


New features:

·	When working with pc2 files I always missed the ability to know beforehand the start and end frame of the file. Now the node supports this. There are two new output ports that furnish the start and end frames as scalars. You can use this to do time warps, offsets, view them in viewports as custom attributes or whatever else you might find it useful for.

