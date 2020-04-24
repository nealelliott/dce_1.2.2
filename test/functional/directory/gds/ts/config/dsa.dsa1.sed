:*****TEST  (CREATE DATABASE dsa-m1-master) ****:
:*** sccsid = @(#)t8.dsa1	7.2 91/07/19 (K Sys Ap 11) ****:
:directory id:1
:Authentication mechanism to be used:Anonymous
:options:Logon to the Default DSA
:****Administration ****:
:Function:1
:****AddObject  de *******:
:Operation:01
:Objekttypnummer:02
:country:de
:ObjectClass:C
:Auxiliary Object Class:NO
:Attribute Name1:
:Attribute Name2:
:Attribute Name3:
:Attribute Name4:
:Attribute Name5:
:More:
:****AddObject  de/dbp *******:
:Operation:01
:Objekttypnummer:03
:country:de
:organization:dbp
:ObjectClass:Organization
:Auxiliary Object Class:NO
:Attribute Name1:
:Attribute Name2:
:Attribute Name3:
:Attribute Name4:
:Attribute Name5:
:More:
:****AddObject  de/dbp/dap11 *******:
:Operation:01
:Objekttypnummer:04
:country:de
:organization:dbp
:OrganizationalUnit:dap11
:ObjectClass:Organizational-Unit
:Auxiliary Object Class:NO
:Attribute Name1:
:Attribute Name2:
:Attribute Name3:
:Attribute Name4:
:Attribute Name5:
:More:
:****AddObject de/dbp/dap11/dsa *******:
:Operation:01
:Objekttypnummer:05
:country:de
:organization:dbp
:OrganizationalUnit:dap11
:User:dsa
:ObjectClass:APP
:Auxiliary Object Class:NO
:Attribute Name1:
:Attribute Name2:
:Attribute Name3:
:Attribute Name4:
:Attribute Name5:
:More:
:****AddObject  de/dbp/dap11/dsa/dsa-m1 *******:
:Operation:01
:Objekttypnummer:07
:country:de
:organization:dbp
:OrganizationalUnit:dap11
:User:dsa
:DSA-Name:dsa-m1
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
:Net-address 1:TCP/IP!internet=SERVER_IP+port=21011
:Net-address 2:
:Net-address 3:
:Net-address 4:
:Net-address 5:
:******** RETURN ******:
:operation:00
:**** Administration ****:
:Function:4
:****SAVE SUBTREE*******:
:operation:1
:Objekttypnummer:02
:object type:OBJECT AND ITS SUBORDINATES
:country:de
:Source DSA:BIND DSA
:filename:save1
:filename:
:**** END ****:
:Operation:00
:****END TEST****:
:Operation:00
