:*****TEST  (CREATE DATABASE dsa-m2-shadow) ****:
:*** sccsid = @(#)t8.dsa2	7.2 91/07/19 (K Sys Ap 11) ****:
:directory id:1
:Authentication mechanism to be used:Anonymous
:options:Logon to a Specific DSA
:***** DSA-Identifikation *****:
:country:de
:organization:dbp
:organizational unit:dap11
:User:dsa
:DSA-name:dsa-m2
:options:None
:****Subtree Administration ****:
:Function:4
:****APPEND SUBTREE*******:
:operation:2
:Objekttypnummer:0
:filename:save1
:filename:
:overwrite:Y
:ACL:Original
:sink DSA:BIND
:operation:0
:**** Administration ****:
:Function:1
:****AddObject  de/dbp/dap11/dsa-m2 *******:
:Operation:01
:Objekttypnummer:07
:country:de
:organization:dbp
:OrganizationalUnit:dap11
:User:dsa
:User:dsa-m2
:ObjectClass:Directory-Service-Agent
:Auxiliary Object Class:NO
:Attribute Name1:Presentation-Address
:Attribute Name2:User-Password
:Attribute Name3:Authentication-Mechanism
:Attribute Name4:
:Attribute Name5:
:More:
:attributName:User-Password
:attributwert:dsa'
:attributwert:
:attributName:Authentication-Mechanism
:attributwert:2'
:attributwert:
:attributName:
:attributwert:
:attributwert:
:P-Selector:
:S-Selector:
:T-Selector:Server
:Net-address 1:TCP/IP!internet=SERVER_IP+port=21012
:Net-address 2:
:Net-address 3:
:Net-address 4:
:Net-address 5:
:**** END ****:
:Operation:00
:****END TEST****:
:Operation:00
