shell "copy c:\dfacltp\fc.fil u:\d1-05-07" > c:\dfacltp\output\c0507001.out
shell "wait 5"
shell "fc c:\dfacltp\fc.fil u:\d1-05-07\fc.fil" >> c:\dfacltp\output\c0507001.out
@echo off
