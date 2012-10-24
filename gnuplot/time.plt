reset
set term pdf size 6,4
set output 'time.pdf'
set xtics ("0" 0,"20" 40000,"40" 80000,"60" 120000,"80" 160000,"100" 200000)
set xrange[0:200000]
set yrange[-100:0]
unset key

set multiplot
set origin 0.0,0.5
set size 1.0,0.5
set xtics font ',10'
set ytics 20 font ',10' border 
set ylabel 'RSS(dBm)' font ",12"
unset xlabel
set arrow from 60000,-85 to 60000,-20 nohead ls 9 lt 9 lw 8
set arrow from 175000,-65 to 165000,-75 lw 4
#set object 1 rectangle from 64000,-50 to 69000,-70 lw 2
#set object 2 rectangle from 122000,-50 to 132000,-80 lw 2
set object 3 rectangle from 150000,-70 to 170000,-90 lw 2 back
set label 1 "LOS" at 36000,-25 font ',10'
set label 2 "NLOS" at 66000,-25 font ',10'
plot 'time' using 13:12 w p pt 7 ps 0.3 lt 7

set origin 0.68,0.68
#reset 
unset xlabel
unset ylabel
unset arrow
unset label 1
unset label 2
set size 0.3,0.3
set xrange[150000:170000]
set yrange[-85:-75]
unset xtics
set ytics 5 font ',8'
plot 'time' u 13:12 s sbezier lt 3 lw 4,'' u 13:12 w p pt 6 ps 0.5 lt 9

set origin 0.0,0.0
reset
unset key
set size 1.0,0.5
set xrange[0:200000]
set yrange[0:100]
set xtics ("0" 0,"20" 40000,"40" 80000,"60" 120000,"80" 160000,"100" 200000) font ',10'
set ytics 20 font ',10'
set ylabel 'PDR(%)' font ",12"
set xlabel 'Time(s)' font ",12"
plot 'time' using 13:4 w p pt 1 ps 0.5 lt 1
unset multiplot
set output
