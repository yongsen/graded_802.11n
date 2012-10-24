graded_802.11n
==============

Packet Delivery Modeling in Mobile 802.11n Networks


Brief Description

Graded 802.11n is composed of packet delivery measurement on both network layer and device layer.

The network layer conducts DSWA calculations to determine averaging intervals and sliding factor, and makes GradedT update to get rate selection indexes.

On the device layer, it is driven by transmitting and receiving events that execute PDR computation and RSS averaging respectively. The rate indexes are also selected on device layer according to results of network layer when the PDR or RSS is lower than transmission threshold.


Files

data: measured data including packet loss rate, packet failed rate, received signal strength, etc.

DSWA: dynamic sliding window averaging

gnuplot: demos of gnuplot of experimental results

pktinfo: packet delivery measurement source files based on ath9k


How to


For DSWA

cd DSWA

sudo make

sudo DSWA


For pktinfo

make sure the device information in common.h is exactly what you are going to track

cd pktinfo

sudo make

sudo pktinfo


For gnuplot

make sure the experimental results and time.plt are in the same folder

gnuplot

load time.plt