:*************************************************************************:
:****                                                                *****:
:**** INITIALIZATION BATCHFILE                                       *****:
:**** Run  "gdssysadm -fc2 -d11 -C16 -A25" before                    *****:
:****  1. initialize cache of dir ID 11                              *****:
:****                                                                *****:
:*************************************************************************:
:**** INITIALIZATION BATCHFILE *****:
:directory id:11
:Authentication mechanism to be used:Anonymous
:options() Logon to DUA Cache :  '
:****Administration ****:
:Function Object Administration:1
:****AddObject  re/sni/buba/dsa/dsa1 *******:
:Operation:01
:Objekttypnummer:07
:country:re
:organization:sni
:OrganizationalUnit:buba
:Common-Name:dsa
:Common-Name:dsa1
:ObjectClass:DSA
:Auxiliary Object class:NO
:Attribute Name1:Presentation-Address
:Attribute Name2:DSA-Type
:Attribute Name3:
:Attribute Name4:
:Attribute Name5:
:More:
:attributName:DSA-Type
:attributwert:default/local'
:attributwert:
:attributName:
:attributwert:
:attributwert:
:attributName:
:attributwert:
:attributwert:
:****attributName Principal Name ******:
:attributName:Principal-Name
:attributwert:/.../c=ie/o=digital/gds-dsa1
:attributwert:
:P-Selector:
:S-Selector:
:T-Selector:Server
:Net-address 1:TCP/IP!internet=127.0.0.1+port=21111 
:Net-address 2:
:Net-address 3:
:Net-address 4:
:Net-address 5:
:****AddObject  re/sni/buba/dsa/dsa2 *******:
:Operation:01
:Objekttypnummer:07
:country:re
:organization:sni
:OrganizationalUnit:buba
:Common-Name:dsa
:Common-Name:dsa2
:ObjectClass:DSA
:Auxiliary Object class:NO
:Attribute Name1:Presentation-Address
:Attribute Name2:
:Attribute Name3:
:Attribute Name4:
:Attribute Name5:
:More:
:P-Selector:
:S-Selector:
:T-Selector:Server
:Net-address 1:TCP/IP!internet=127.0.0.1+port=21121 
:Net-address 2:
:Net-address 3:
:Net-address 4:
:Net-address 5:
:******** RETURN ******:
:operation:00
:**** Administration ****:
:Function:0
