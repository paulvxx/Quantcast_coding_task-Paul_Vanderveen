# Quantcast_coding_task-Paul_Vanderveen
Completed coding task for Quantcast Interview

Source File(s):

most_active_cookie.c

Compile: 

gcc most_active_cookie.c -o most_active_cookie

Program Running and Command Line Usage:

./most_active_cookie [cookie log file] -d [date in yyyy-mm-dd form]

Given a cookie log file in the following format:

cookie,timestamp

Example:

cookie_log.csv:


AtY0laUfhglK3lC7,2018-12-09T14:19:00+00:00

SAZuXPGUrfbcn5UA,2018-12-09T10:13:00+00:00

5UAVanZf6UtGyKVS,2018-12-09T07:25:00+00:00

AtY0laUfhglK3lC7,2018-12-09T06:19:00+00:00

SAZuXPGUrfbcn5UA,2018-12-08T22:03:00+00:00

4sMM2LxV07bPJzwf,2018-12-08T21:30:00+00:00

fbcn5UAVanZf6UtG,2018-12-08T09:30:00+00:00

4sMM2LxV07bPJzwf,2018-12-07T23:30:00+00:00


The program prints the most active cookie(s) for the specified date. If there are no cookie 
entries for a specified date, then no output is returned.


Examples:


./most_active_cookie cookie_log.csv -d 2018-12-08

SAZuXPGUrfbcn5UA

4sMM2LxV07bPJzwf

fbcn5UAVanZf6UtG

./most_active_cookie cookie_log.csv -d 2018-12-09

AtY0laUfhglK3lC7

./most_active_cookie cookie_log.csv -d 2018-12-06

(no output)

For a list of running tests, please see command_tests.txt

