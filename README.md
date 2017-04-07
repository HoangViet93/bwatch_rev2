# bwatch_rev2
Hey, it's remake version of my old project [bwatch](https://github.com/HoangViet93/beewatch). 
I re-design it by use `Makefile` and `gcc` instead of KeilC and cut-off some uncomplete feature like 
zigbee and focus to most useful things. </br>
Here are some things my watch can do:
- measure heart rate
- stopwatch, analog and digital clock
- activity status
# To used it ?
install arm embedded toolchain frist, after that go to any folder in unit-test and `make lib` and `make`.
I seperate make to two phase just because i want reduce the log messgae.
